#pragma once

#include "BismuthComponent.h"
#include "../Data/BismuthData.h"

#include <memory>

namespace bismuth
{

	class ComponentPool
	{
	public:
		ComponentPool(ComponentVTable component)
			: m_VTable(component)
		{}

		/*
		 * Constructs the data for a new component and ties it to an entity id 
		 */
		void InitializeComponent(EntityID entity);

		/*
		 * Removes the data of a component for a specified entity.
		 * Swaps the Data of the select entities data with the data of the last entity then deletes and pops the data from the vector.
		 */
		void DeleteComponent(EntityID entity);

		/*
		 * Accesses a specific piece of data tied to an entity id.
		 */
		IDataInstance* const GetProperty(EntityID entity, PropertyID property) const;

		/*
		 * Changes the value of a property. 
		 */
		void SetProperty(EntityID entity, PropertyID property, DataPtr newVal);

		inline ComponentVTable GetVTable() const { return m_VTable; }

	private:
		ComponentVTable m_VTable;
		std::vector<IDataInstance*> m_DataPool;
		std::map<EntityID, size_t> m_EntityLocations;
	};

}