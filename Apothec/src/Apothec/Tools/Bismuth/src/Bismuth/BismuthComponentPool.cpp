#include "BismuthComponentPool.h"

namespace bismuth
{

	void
	ComponentPool::InitializeComponent(EntityID entity)
	{
		m_EntityLocations[entity] = m_DataPool.size();
		m_VTable->Construct(m_DataPool, entity);
	}

	void
	ComponentPool::DeleteComponent(EntityID entity)
	{
		if (m_EntityLocations.count(entity) > 0) // assert entities existence.
		{
			EntityID lastEntity = m_DataPool[m_DataPool.size() - m_VTable->GetAlignment()]->GetAs<unsigned long long>();
			size_t entityLocation = m_EntityLocations[entity];
			size_t lastEntityLocation = m_EntityLocations[lastEntity];

			// swaps the data of the top entity and the select entity
			for (size_t i{}; i < m_VTable->GetAlignment(); ++i)
			{
				delete m_DataPool[entityLocation + i];
				m_DataPool[entityLocation + i] = m_DataPool[lastEntityLocation + i];
			}

			for (size_t i{}; i < m_VTable->GetAlignment(); ++i) // remove the moved data 
				m_DataPool.pop_back();

			m_EntityLocations[lastEntity] = entityLocation; // update moved entity's pointer
		}
	}

	IData* const 
	ComponentPool::GetProperty(EntityID entity, PropertyID property)
	const
	{
		if (m_EntityLocations.count(entity) > 0)
			return m_VTable->GetProperty(m_DataPool, m_EntityLocations.at(entity), property);
	}

	void
	ComponentPool::SetProperty(EntityID entity, PropertyID property, DataPointer newVal)
	{
		if (m_EntityLocations.count(entity) > 0)
			 m_VTable->SetProperty(m_DataPool, m_EntityLocations.at(entity), property, newVal);
	}


}