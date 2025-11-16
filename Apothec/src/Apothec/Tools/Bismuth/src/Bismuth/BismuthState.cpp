#include "BismuthState.h"

#include <random>
#include <ctime>

namespace bismuth
{

	state* state::s_Instance = nullptr;

	state::state()
	{
		srand(time(0));
		s_Instance = this;
	}

	EntityID
	state::CreateUniqueEntityID()
	const
	{
		EntityID id = rand();
		while (m_Entities.count(id) > 0)
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

}