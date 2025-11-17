#include "BismuthState.h"

#include <random>
#include <ctime>
#include <cassert>

namespace bismuth
{

	state* state::s_Instance = nullptr;

	state::state()
	{
		srand((unsigned int)time(0));
		s_Instance = this;
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
				++index; // to identifier
				c_token = tokens[index];
				assert(c_token.Type == generation::TokenType::Identifier && "Unexpected token following component declaration, expected identifier.");
				assert(m_ComponentPools.count(c_token.Value.value()) == 0 && "Component already defined. Try adding a header guard or check for components with the same name declared in included modules or loaded projects.");
				const PropertyID id = c_token.Value.value();

				std::vector<PropertyTemplate> props;
				std::vector<std::pair<PropertyID, IFunction*>> methods;

				++index;
				c_token = tokens[index];
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
					{

					}

					else
						assert("Unexpected token found in component declaration");

					++index;
					c_token = tokens[index];
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
				++index; // to identifier
				c_token = tokens[index];
				assert(c_token.Type == generation::TokenType::Identifier && "Unexpected token following archetype declaration, expected identifier");

				PropertyID archID = c_token.Value.value();

				++index; // to inclusion
				c_token = tokens[index];
				assert(c_token.Type == generation::TokenType::ComponentInclusion && "Expected \"with\" token following archetype identifier");

				std::vector<PropertyID> components;
				std::map<PropertyID, IFunction*> methods;

				++index;
				c_token = tokens[index];
				while (c_token.Type == generation::TokenType::Identifier)
				{
					assert(m_ComponentPools.count(c_token.Value.value()) > 0 && "Component included by archetype does not exist, check spelling or if the component you are trying to include is not properly added");
					components.push_back(c_token.Value.value());
					++index; 
					c_token = tokens[index];
				}

				assert(c_token.Type == generation::TokenType::ScopeStart || c_token.Type == generation::TokenType::LineEnd && "Unexpected token following archetype signature, try adding a ; if the archetype has no methods otherwise add a scope {}.");
				if (c_token.Type == generation::TokenType::ScopeStart)
				{
					// parse for functions
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

			}

			/*
			 * Entity Tags:
			 * - Requires an identifier and at least one component or one archetype.
			 * - Can optionally have a list of systems and interfaces.
			 * - The entity can have multiple constructors as well as methods and implementations for interface functions.
			 */
			else if (c_token.Type == generation::TokenType::EntityTag)
			{

			}

			/*
			 *  Function Tags:
			 * - Global function.
			 */
			else if (c_token.Type == generation::TokenType::FunctionTag)
			{

			}


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
				PUSHINDEX;
			}

		}

		PUSHINDEX;


	}

}