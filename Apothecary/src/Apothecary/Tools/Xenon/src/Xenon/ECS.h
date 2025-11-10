#pragma once

#include "ComponentPool.h"
#include "SystemInterface.h"

#include<string>
#include<unordered_map>
#include<unordered_set>
#include<bitset>

namespace xenon
{
	struct Entity_Container
	{
		XenonArchetype components = {};
		bool tickEnabled = false;
	};

	class registry
	{
	public:
		registry(unsigned int max, unsigned int maxSystems, unsigned int maxArchetypes)
			: m_MaxEntities(max)
			, m_MaxSystems(maxSystems)
			, m_MaxArchetypes(maxArchetypes)
		{
			m_Systems = (SystemInterface**)malloc(maxSystems * sizeof(SystemInterface*));
			m_Archetypes = (XenonArchetype*)malloc(maxArchetypes * sizeof(XenonArchetype));
		}

		~registry()
		{
			free(m_Systems);
			free(m_Archetypes);
			m_Systems = nullptr;
			m_Archetypes = nullptr;
		}

		/*Creates an entity and stores it within the ecs registry*/
		XenonID CreateEntity()
		{
			XenonID loc = (XenonID)m_Top;
			++m_Top;
			return loc;
		}
		
		/*Creates the necessary pools for a component and validates it for use by entities*/
		template<typename _T>
		void RegisterComponent()
		{
			m_ComponentTypes[typeid(_T).name()] = (unsigned int)m_RegisteredComponents;
			m_Pools[(XenonID)m_RegisteredComponents] = new ComponentPool<_T>((XenonID)m_MaxEntities);
			++m_RegisteredComponents;
		}
		/*Add a component to an entity*/
		template<typename _T>
		void AddComponent(XenonID entity, _T* component)
		{
			dynamic_cast<ComponentPool<_T>*>(m_Pools[m_ComponentTypes[typeid(_T).name()]])->add(entity, component);
			m_Entities[entity].components.set(m_ComponentTypes[typeid(_T).name()]);
		}
		/*Returns if an entity has the desired component*/
		template<typename _T>
		bool HasComponent(XenonID id)
		{
			return m_Entities[id].components.test(m_ComponentTypes[typeid(_T).name()]);
		}
		/*Gets a pointer to the desired component of an entity. Recommended to be used alongside HasComponent or check the return value as it will return nullptr if not found.*/
		template<typename _T>
		_T* GetComponent(XenonID id)
		{
			ComponentPool<_T>* pool = dynamic_cast<ComponentPool<_T>*>(m_Pools[m_ComponentTypes[typeid(_T).name()]]);
			std::optional<_T*> comp = pool->GetComponent(id);
			if (comp.has_value())
				return comp.value();
			else
				return nullptr;
		}
		/*Removes a component from an entity*/
		template<typename _T>
		bool RemoveComponent(XenonID id)
		{
			ComponentPool<_T>* pool = dynamic_cast<ComponentPool<_T>*>(m_Pools[m_ComponentTypes[typeid(_T).name()]]);
			if (pool->contains(id))
			{
				pool->remove(id);
				m_Entities[id].components.flip(m_ComponentTypes[typeid(_T).name()]);
				return true;
			}
			return false;
		}
		/*Destroys an entity. Should be pushed to a que as it will mess up entity iteration */
		void DestroyEntity(XenonID id)
		{
			XenonArchetype comps = m_Entities[id].components; // destroy components

			for (unsigned int i{}; i < m_RegisteredComponents; ++i)
			{
				if (comps.test(i))
					m_Pools[i]->remove(id);
			}

			XenonID endID = (XenonID)m_Top; // move the top entity to the removed entity location
			comps = m_Entities[endID].components;
			for (unsigned int i{}; i < m_RegisteredComponents; ++i)
			{
				if (comps.test(i))
					m_Pools[i]->move(endID, id);
			}
			m_Entities[id] = m_Entities[endID];
			--m_Top;
		}
		/*Enables tick updates for systems related to this entities archetype.*/
		void SetTickEnabled(XenonID id, bool val = true)
		{
			m_Entities[id].tickEnabled = val;
		}




		/*Register an archetype. Template takes in a System and the components it will update.*/
		template<typename _System, typename... _Components>
		void RegisterArchetype()
		{
			if (m_TotalSystems < m_MaxSystems && m_TotalArchetypes < m_MaxArchetypes)
			{
				XenonArchetype archetype = ArchetypeKey<_Components...>();
				m_Systems[m_TotalSystems] = new _System{ archetype };
				m_Archetypes[m_TotalArchetypes] = archetype;
				m_ArchetypeSystems[archetype] = (unsigned int)m_TotalSystems;
				++m_TotalSystems;
				++m_TotalArchetypes;
			}
		}
		/*Register an alternate architype for the previously registered system.*/
		template<typename... _Components>
		void RegisterAlternate()
		{
			XenonArchetype arch = ArchetypeKey<_Components...>();
			m_ArchetypeSystems[arch] = (unsigned int)m_TotalSystems - 1;
			m_Archetypes[m_TotalArchetypes] = arch;
			++m_TotalArchetypes;
		}
		/*Calls updates on all entities with tick enabled. A system matching an entities architype must exist for an update to be called.*/
		void UpdateSystems()
		{
			for (XenonID ent = 0; ent < m_Top; ++ent)
				if (m_Entities[ent].tickEnabled)
					for (unsigned int arch{}; arch < m_TotalArchetypes; ++arch)
						if ((m_Entities[ent].components & m_Archetypes[arch]) == m_Archetypes[arch])
						m_Systems[m_ArchetypeSystems[m_Entities[ent].components]]->OnSystemUpdate(this, ent);
		}

	private:
		/*Generates an architype key from a list of components*/
		template <typename _Current, typename... _Others>
		XenonArchetype ArchetypeKey(XenonArchetype base = {})
		{
			XenonArchetype set = base;
			if (typeid(_Current) != typeid(std::nullptr_t))
			{
				set.set(m_ComponentTypes[typeid(_Current).name()]);
				return ArchetypeKey<_Others..., std::nullptr_t>(set);
			}
			return base;
		}

	private:

		unsigned int m_MaxEntities; // maximum entities
		unsigned int m_Top = 0; // top of entity stack

		std::unordered_map<unsigned int, IComponentPool*> m_Pools; // maps bit location to component pool
		std::unordered_map<std::string, unsigned int> m_ComponentTypes; // maps component name to bit location
		unsigned int m_RegisteredComponents = 0; // the total amount of registered components

		std::unordered_map<XenonID, Entity_Container> m_Entities; // maps each entity id to the components it has and its scripts

		SystemInterface** m_Systems; // Pointers to all registered systems.
		std::unordered_map<XenonArchetype, unsigned int> m_ArchetypeSystems; // maps architype keys to systems.
		XenonArchetype* m_Archetypes; // all of the archetypes
		unsigned int m_TotalArchetypes = 0; // the total amount of archetypes
		unsigned int m_MaxArchetypes; // the maximum amount of archetypes
		unsigned int m_TotalSystems = 0; // the total amount of systems registered
		unsigned int m_MaxSystems; // the maximum amount of systems that can be registered.
	};
}