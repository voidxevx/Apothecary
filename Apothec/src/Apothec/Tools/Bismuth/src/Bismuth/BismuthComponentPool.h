#pragma once

#include "BismuthComponent.h"

#include <memory>

namespace bismuth
{

	class ComponentPool
	{
	public:
		ComponentPool(std::shared_ptr<ComponentVTable> component)
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
		IData* const GetProperty(EntityID entity, PropertyID property) const;

		/*
		 * Changes the value of a property. 
		 */
		void SetProperty(EntityID entity, PropertyID property, DataPointer newVal);

	private:
		std::shared_ptr<ComponentVTable> m_VTable;
		std::vector<IData*> m_DataPool;
		std::map<EntityID, size_t> m_EntityLocations;
	};

}