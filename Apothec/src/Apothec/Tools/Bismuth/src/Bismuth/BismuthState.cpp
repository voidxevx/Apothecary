#include "BismuthState.h"

#include <random>
#include <ctime>

namespace bismuth
{

	state* state::s_Instance = nullptr;

	state::state()
	{
		srand((unsigned int)time(0));
		s_Instance = this;
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
		for (const auto& comp : m_EntityVTables[vtable]->GetComponents())
			m_ComponentPools[comp]->InitializeComponent(id);

		// initialize archetype components
		for (const auto& arch : m_EntityVTables[vtable]->GetArchetypes())
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
		for (const auto& compPool : m_ComponentPools)
			compPool.second->DeleteComponent(entity);

		for (const auto& syst : m_EntityVTables[m_Entities[entity]]->GetSystems())
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
				}

			});
		}
	}

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

			}

			/*
			 * Archetype Tags:
			 * - Requires an identifier and at least one component.
			 * - stores only methods.
			 */
			else if (c_token.Type == generation::TokenType::ArchetypeTag)
			{

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

}