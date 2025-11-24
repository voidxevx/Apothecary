#include "BismuthState.h"

#include <random>
#include <ctime>
#include <cassert>


/*
	TODO: 
		- extra syntax.
		- assert correct declarations of entities.
*/

namespace bismuth
{

	state* state::s_Instance = nullptr;

	state::state()
	{
		srand((unsigned int)time(0));
		s_Instance = this;

		m_IOStream = std::make_shared<DataIO>();
	}

	state::~state()
	{
		m_BuildThread.join();
		m_MainThread.join();
	}

	EntityID
	state::CreateUniqueEntityID()
	const
	{
		EntityID id = rand();
		while (m_Entities.count(id) != 0)
			id = rand();
		return id;
	}

	EntityID
	state::CreateEntity(PropertyID vtable)
	{
		EntityID id = CreateUniqueEntityID();

		m_Entities[id] = vtable;

		// initialize primary components
		for (auto& comp : m_EntityVTables[vtable]->GetComponents())
			m_ComponentPools[comp]->InitializeComponent(id);

		// initialize archetype components
		for (auto& arch : m_EntityVTables[vtable]->GetArchetypes())
			for (const auto& comp : m_ArchetypeVTables[arch]->GetComponents())
				m_ComponentPools[comp]->InitializeComponent(id);

		// register systems
		for (const auto& syst : m_EntityVTables[vtable]->GetSystems())
			m_Systems[syst]->RegisterEntity(id);

		return id;
	}

	void
	state::DestroyEntity(EntityID entity)
	{
		for (auto& compPool : m_ComponentPools)
			compPool.second->DeleteComponent(entity);

		for (auto& syst : m_EntityVTables[m_Entities[entity]]->GetSystems())
			m_Systems[syst]->RemoveEntity(entity);
	}


	void state::BuildFile(const std::string& filePath)
	{
		if (m_BuildThread.joinable())
			m_BuildQueue.push_back(filePath);
		else
		{
			m_BuildQueue.push_back(filePath);
			m_BuildThread = std::thread([&]() {

				while (m_BuildQueue.size() > 0)
				{
					generation::Tokenizer tokens{ m_BuildQueue.back() };
					BuildTokens(tokens);
					m_BuildQueue.pop_back();
				}

			});
		}
	}

#define PUSHINDEX ++index; c_token = tokens[index]

	void
	state::BuildTokens(generation::Tokenizer& tokenizer)
	{
		
		const std::vector<generation::Token>& tokens{ tokenizer.GetTokens() };
		size_t index{};
		while (index < tokens.size())
		{
			generation::Token c_token{ tokens[index] };

			/*
			 * Project tags - checks if a project has already been built. 
			 */
			if (c_token.Type == generation::TokenType::ProjectTag)
			{
				++index;
				assert(tokens[index].Type == generation::TokenType::Identifier && "Unexpected token following project header guard, expected identifier.");
				if (std::find(m_Projects.begin(), m_Projects.end(), tokens[index].Value.value()) != m_Projects.end())
					return;
				else
					m_Projects.push_back(tokens[index].Value.value());
			}

			/*
			 * Components Tags:
			 *  - Requires an identifier to follow.
			 *  - stores properties and methods.
			 */
			else if (c_token.Type == generation::TokenType::ComponentTag)
			{
				PUSHINDEX; // to identifier
				assert(c_token.Type == generation::TokenType::Identifier && "Unexpected token following component declaration, expected identifier.");
				const PropertyID id = c_token.Value.value();
				assert(m_ComponentPools.count(id) == 0 && "Component already defined. Try adding a header guard or check for components with the same name declared in included modules or loaded projects.");

				std::vector<PropertyTemplate> props;
				std::vector<std::pair<PropertyID, IFunction*>> methods;

				PUSHINDEX; // to scope start
				assert(c_token.Type == generation::TokenType::ScopeStart && "Unexpected token after component declaration, expected scope start");
				PUSHINDEX; // to first token

				while (c_token.Type != generation::TokenType::ScopeEnd)
				{
					
					/*
					 * properties have two identifiers: the type and the name.
					 */
					if (c_token.Type == generation::TokenType::Identifier && tokens[index + 1].Type == generation::TokenType::Identifier)
					{
						props.push_back(PropertyTemplate{ c_token.Value.value(), tokens[index + 1].Value.value() });
						++index;
					}

					/*
					 * parse function
					 */
					else if (c_token.Type == generation::TokenType::FunctionTag)
						methods.push_back(ParseFunction(tokens, index));

					else
						assert(false && "Unexpected token found in component declaration");

					PUSHINDEX;
				}

				m_ComponentPools[id] = new ComponentPool( ComponentVTable{ methods, props } );
			}

			/*
			 * Archetype Tags:
			 * - Requires an identifier and at least one component.
			 * - stores only methods.
			 */
			else if (c_token.Type == generation::TokenType::ArchetypeTag)
			{
				PUSHINDEX;
				assert(c_token.Type == generation::TokenType::Identifier && "Unexpected token following archetype declaration, expected identifier");

				PropertyID archID = c_token.Value.value();
				assert(m_ArchetypeVTables.count(archID) == 0 && "Archetype already defined. Try adding a header guard or check for components with the same name declared in included modules or loaded projects.");

				PUSHINDEX; // to inclusion
				assert(c_token.Type == generation::TokenType::ComponentInclusion && "Expected inclusion token (with) following archetype identifier");

				std::vector<PropertyID> components;
				std::map<PropertyID, IFunction*> methods;

				PUSHINDEX;
				while (c_token.Type == generation::TokenType::Identifier)
				{
					assert(m_ComponentPools.count(c_token.Value.value()) > 0 && "Component included by archetype does not exist, check spelling or if the component you are trying to include is not properly added");
					components.push_back(c_token.Value.value());
					PUSHINDEX;
				}

				assert(c_token.Type == generation::TokenType::ScopeStart || c_token.Type == generation::TokenType::LineEnd && "Unexpected token following archetype signature, try adding a ; if the archetype has no methods otherwise add a scope {}.");
				if (c_token.Type == generation::TokenType::ScopeStart)
				{
					
					PUSHINDEX;
					while (c_token.Type != generation::TokenType::ScopeEnd)
					{
						assert(c_token.Type == generation::TokenType::FunctionTag);
						std::pair<PropertyID, IFunction*> method = ParseFunction(tokens, index);
						methods[method.first] = method.second;
						PUSHINDEX;
					}

				}

				m_ArchetypeVTables[archID] = new ArchetypeVTable(components, methods);
			}

			/*
			 * Interface Tags:
			 * - Requires an identifier and at least one component or archetype.
			 * - cannot store function implementations only signatures
			 */
			else if (c_token.Type == generation::TokenType::InterfaceTag)
			{
				PUSHINDEX; 
				assert(c_token.Type == generation::TokenType::Identifier && "Unexpected token following interface tag, expected identifier.");
				PropertyID id = c_token.Value.value();
				assert(m_InterfaceVtables.count(id) == 0 && "Interface already defined. Try adding a header guard or check for interfaces with the same name declared in included modules or loaded projects.");

				PUSHINDEX;
				assert(c_token.Type == generation::TokenType::ComponentRequirement && "interfaces must be followed by a requirement tag before listing component.");
				PUSHINDEX;

				std::vector<PropertyID> components;

				while (c_token.Type == generation::TokenType::Identifier)
				{
					components.push_back(c_token.Value.value());
					// TODO: assert component or archetype exits
					PUSHINDEX;
				}

				assert(components.size() >= 1 && "Interfaces must require at least one or more components or archetypes.");
				assert(c_token.Type == generation::TokenType::ScopeStart && "unexpected token following interface signature, expected scope");
				PUSHINDEX;

				std::vector<FunctionImplementation> methods;

				while (c_token.Type == generation::TokenType::FunctionTag)
				{
					PUSHINDEX; // to identifier
					assert(c_token.Type == generation::TokenType::Identifier && "Unidentified token following interface function, expected identifier.");
					PropertyID methodID = c_token.Value.value();
					PUSHINDEX;
					
					std::vector<PropertyTemplate> inputs;

					if (c_token.Type == generation::TokenType::ExpressionStart)
					{
						PUSHINDEX;

						while (c_token.Type != generation::TokenType::ExpressionEnd)
						{
							assert(c_token.Type == generation::TokenType::Identifier && tokens[index + 1].Type == generation::TokenType::Identifier && "Unexpected token found in function input list");

							inputs.push_back(PropertyTemplate{ c_token.Value.value(), tokens[index].Value.value() });

							// TODO: add expression break syntax

							++index;
							PUSHINDEX;
						}
						PUSHINDEX;
					}


					PropertyID retType = generation::Tokenizer::GetHasher()("void");
					if (c_token.Type == generation::TokenType::ReturnHint)
					{
						PUSHINDEX;
						assert(c_token.Type == generation::TokenType::Identifier && "Unexpected token as return hint, expected identifier");
						retType = c_token.Value.value();

						PUSHINDEX; // to end line
					}

					assert(c_token.Type == generation::TokenType::LineEnd && "Unexpected token following interface function signature, expected line end");
					PUSHINDEX;

					methods.push_back({ methodID, retType, inputs });
				}

				assert(methods.size() >= 1 && "Interfaces must have at least one function signature declared.");
				assert(c_token.Type == generation::TokenType::ScopeEnd && "Unexpected token within interface, expected function tag or scope end");

				m_InterfaceVtables[id] = new InterfaceVTable(components, methods);
			}

			/*
			 * System Tags:
			 * - Requires An identifier.
			 * - Can optionally include required archetypes.
			 * - cannot include single components.
			 * - Stores a single function.
			 */
			else if (c_token.Type == generation::TokenType::SystemTag)
			{
				PUSHINDEX;
				assert(c_token.Type == generation::TokenType::Identifier && "Unexpected token following system tag, expected identifier");
				const PropertyID id = c_token.Value.value();
				assert(m_Systems.count(id) == 0 && "System already defined. Try adding a header guard or check for systems with the same name declared in included modules or loaded projects.");

				std::vector<PropertyID> archetypes;

				PUSHINDEX;
				if (c_token.Type == generation::TokenType::ArchetypeInclusion)
				{
					PUSHINDEX;

					while (c_token.Type == generation::TokenType::Identifier)
					{
						const PropertyID archID = c_token.Value.value();
						assert(m_ArchetypeVTables.count(archID) > 0 && "Archetype required by system does not exist. Make sure that the archetype you are attempting to require is properly included.");
						archetypes.push_back(archID);
						PUSHINDEX;
					}

				}

				assert(c_token.Type == generation::TokenType::ScopeStart && "Unexpected token following system signature, expected scope.");
				PUSHINDEX;
				while (c_token.Type != generation::TokenType::ScopeEnd)
				{
					// TODO: parse bytecode

					PUSHINDEX;
				}

				m_Systems[id] = new LocalSystem(archetypes);
			}

			/*
			 * Entity Tags:
			 * - Requires an identifier and at least one component or one archetype.
			 * - Can optionally have a list of systems and interfaces.
			 * - The entity can have multiple constructors as well as methods and implementations for interface functions.
			 * 
			 * - Compile time check that the entity includes the correct component for its attached systems and interfaces
			 */
			else if (c_token.Type == generation::TokenType::EntityTag)
			{
				PUSHINDEX;
				assert(c_token.Type == generation::TokenType::Identifier && "Unexpected token following entity tag, expected identifier.");
				const PropertyID id = c_token.Value.value();
				assert(m_EntityVTables.count(id) == 0 && "Entity already defined. Try adding a header guard or check for entity with the same name declared in included modules or loaded projects.");
				
				std::vector<PropertyID> components;
				std::vector<PropertyID> archetypes;

				PUSHINDEX;
				if (c_token.Type == generation::TokenType::ComponentInclusion)
				{
					PUSHINDEX;

					while (c_token.Type == generation::TokenType::Identifier)
					{
						const PropertyID compID = c_token.Value.value();
						assert(m_ComponentPools.count(compID) > 0 && "Component included by entity does not exist, make sure that the component is spelled correctly or correctly included.");
						components.push_back(compID);
						PUSHINDEX;
					}

				}

				if (c_token.Type == generation::TokenType::ArchetypeInclusion)
				{
					PUSHINDEX;

					while (c_token.Type == generation::TokenType::Identifier)
					{
						const PropertyID archID = c_token.Value.value();
						assert(m_ArchetypeVTables.count(archID) > 0 && "Archetype included by entity does not exist, make sure that the archetype is spelled correctly or correctly included.");
						archetypes.push_back(archID);
						PUSHINDEX;
					}
				}

				assert(components.size() > 0 || archetypes.size() > 0 && "Entities must include at least one component or archetype");

				std::vector<PropertyID> systems;

				if (c_token.Type == generation::TokenType::SystemAccess)
				{
					PUSHINDEX;

					while (c_token.Type == generation::TokenType::Identifier)
					{
						const PropertyID sysID = c_token.Value.value();
						assert(m_Systems.count(sysID) > 0 && "system included by entity does not exist, make sure that the system is spelled correctly or correctly included.");
						// TODO: check component requirements
						systems.push_back(sysID);
						PUSHINDEX;
					}
				}

				std::vector<PropertyID> interfaces;

				if (c_token.Type == generation::TokenType::InterfaceImplementation)
				{
					PUSHINDEX;

					while (c_token.Type == generation::TokenType::Identifier)
					{
						const PropertyID interID = c_token.Value.value();
						assert(m_InterfaceVtables.count(interID) > 0 && "interface included by entity does not exist, make sure that the interface is spelled correctly or correctly included.");
						// TODO: Check interface requirements
						interfaces.push_back(interID);
						PUSHINDEX;
					}
				}

				assert(c_token.Type == generation::TokenType::ScopeStart && "Unexpected token following entity signature, expected scope.");

				std::map<PropertyID, IFunction*> implementations;
				std::vector<IFunction*> constructors;

				PUSHINDEX; // to first token
				while (c_token.Type != generation::TokenType::ScopeEnd)
				{
					if (c_token.Type == generation::TokenType::EntityConstruction)
					{
						PUSHINDEX;

						std::vector<PropertyTemplate> inputs;

						if (c_token.Type == generation::TokenType::ExpressionStart)
						{
							PUSHINDEX;
							while (c_token.Type != generation::TokenType::ExpressionEnd)
							{
								assert(c_token.Type == generation::TokenType::Identifier && tokens[index + 1].Type == generation::TokenType::Identifier && "Unexpected token found in constructor input list");

								inputs.push_back(PropertyTemplate{ c_token.Value.value(), tokens[index].Value.value() });

								// TODO: add expression break syntax

								++index;
								PUSHINDEX; // last iteration -> onto expression end
							}
							PUSHINDEX;
						}

						assert(c_token.Type == generation::TokenType::ScopeStart && "Unexpected token following constructor signature, expected scope.");
						PUSHINDEX;
						while (c_token.Type != generation::TokenType::ScopeEnd)
						{
							// TODO: parse constructor
							PUSHINDEX;
						}

						constructors.push_back(new LocalFunction(0, inputs));

					}
					else if (c_token.Type == generation::TokenType::InterfaceImplementation)
					{
						PUSHINDEX;

						assert(c_token.Type == generation::TokenType::FunctionTag && "unexpected token following interface implementation, expected function");
						PUSHINDEX;
						assert(c_token.Type == generation::TokenType::Identifier && "unexpected token following function tag, expected ");
						const PropertyID funID = c_token.Value.value();

						std::vector<PropertyTemplate> inputs;

						PUSHINDEX;
						if (c_token.Type == generation::TokenType::ExpressionStart)
						{
							PUSHINDEX;
							while (c_token.Type != generation::TokenType::ExpressionEnd)
							{
								assert(c_token.Type == generation::TokenType::Identifier && tokens[index + 1].Type == generation::TokenType::Identifier && "Unexpected token found in function input list");

								inputs.push_back(PropertyTemplate{ c_token.Value.value(), tokens[index].Value.value() });

								// TODO: add expression break syntax

								++index;
								PUSHINDEX; // last iteration -> onto expression end
							}

						}

						assert(c_token.Type == generation::TokenType::ExpressionEnd && "Unexpected token found in function inputs, expected expression end.");
						PUSHINDEX;

						PropertyID retType = generation::Tokenizer::GetHasher()("void");

						if (c_token.Type == generation::TokenType::ReturnHint)
						{
							PUSHINDEX;
							assert(c_token.Type == generation::TokenType::Identifier && "unexpected token following return hint, expected identifier");
							retType = c_token.Value.value();
							PUSHINDEX;
						}

						assert(c_token.Type == generation::TokenType::InterfaceLink && "Unexpected token following interface function implementation signature, expected interface link (from)");
						PUSHINDEX;
						assert(c_token.Type == generation::TokenType::Identifier && "Unexpected token found following interface link, expected identifier.");
						const PropertyID intID = c_token.Value.value();
						assert(m_InterfaceVtables.count(intID) > 0 && "interface included by function does not exist, make sure that the interface is spelled correctly or correctly included.");

						const InterfaceVTable* const& intVTable = m_InterfaceVtables.at(intID);

						const std::vector<FunctionImplementation>& functions = intVTable->GetMethods();

						// TODO: assert function existence and correct implementations

						PUSHINDEX;
						assert(c_token.Type == generation::TokenType::ScopeStart && "Unexpected token following interface function implementation signature, expected scope.");
						PUSHINDEX;
						while (c_token.Type != generation::TokenType::ScopeEnd)
						{
							PUSHINDEX;
						}

						implementations[funID] = new LocalFunction(retType, inputs);
					}
					else
						assert(false && "Unexpected token found in entity declaration.");

					PUSHINDEX;
				}

				m_EntityVTables[id] = new EntityVTable(components, archetypes, systems, interfaces, implementations, constructors);

			}

			/*
			 *  Function Tags:
			 * - Global function.
			 */
			else if (c_token.Type == generation::TokenType::FunctionTag)
			{
				std::pair<PropertyID, IFunction*> func = ParseFunction(tokens, index);
				m_GlobalFunctions[func.first] = func.second;
			}

			else 
				assert(false && "Unexpected floating token");


			++index;
		}

	}

	std::pair<PropertyID, IFunction*> 
	state::ParseFunction(const std::vector<generation::Token>& tokens, size_t& index)
	{
		++index; // to identifier
		generation::Token c_token{ tokens[index] };
		assert(c_token.Type == generation::TokenType::Identifier && "Unexpected token following function tag, expected identifier");
		PropertyID id = c_token.Value.value();

		std::vector<PropertyTemplate> inputs;

		PUSHINDEX;
		if (c_token.Type == generation::TokenType::ExpressionStart)
		{
			PUSHINDEX;
			while (c_token.Type != generation::TokenType::ExpressionEnd)
			{
				assert(c_token.Type == generation::TokenType::Identifier && tokens[index + 1].Type == generation::TokenType::Identifier && "Unexpected token found in function input list");

				inputs.push_back(PropertyTemplate{c_token.Value.value(), tokens[index].Value.value()});

				// TODO: add expression break syntax

				++index;
				PUSHINDEX; // last iteration -> onto expression end
			}

		}

		PUSHINDEX; // to return hint
		PropertyID retType = generation::Tokenizer::GetHasher()("void");

		if (c_token.Type == generation::TokenType::ReturnHint)
		{
			PUSHINDEX;
			assert(c_token.Type == generation::TokenType::Identifier && "Unexpected token as return hint, expected identifier");
			retType = c_token.Value.value();

			PUSHINDEX; // to scope start
		}

		assert(c_token.Type == generation::TokenType::ScopeStart && "Unexpected token following function signature, expected scope");

		while (c_token.Type != generation::TokenType::ScopeEnd)
		{
			PUSHINDEX;
		}

		// ends on scope end

		return { id, new LocalFunction(retType, inputs) };

	}













	/*
		Byte code generation ---------------------
	*/
	#include "runtime/nodes/InstructionNodes.h"

	runtime::INode*
	state::GenerateByteCode(std::vector<generation::Token> tokens)
	{
		using namespace runtime;

		size_t index = 0;
		nodes::Node_Start* const start = new nodes::Node_Start();
		INode* current = start;

		while (index < tokens.size())
		{
			generation::Token c_token = tokens[index];

			++index;
		}

		current->SetNext(new nodes::Node_End());
		return start;
	}




}