#pragma once

#include "Data/types.h"
#include "Components/BismuthComponent.h"
#include "Components/BismuthComponentPool.h"
#include "Data/BismuthDataType.h"
#include "Systems/BismuthFunctions.h"
#include "Systems/BismuthSystem.h"
#include "Entities/BismuthEntity.h"

#include <thread>

namespace bismuth
{

	class state
	{
	public:
		explicit state();
		inline static state* GetGlobal() { return s_Instance; }

		/* Entities ----- */
		EntityID CreateEntity(PropertyID vtable);
		void DestroyEntity(EntityID entity);

		/* Native control ----- */
		inline void RegisterComponent(PropertyID id, std::shared_ptr<ComponentVTable> component) { m_ComponentPools[id] = std::make_unique<ComponentPool>(component); }
		inline void RegisterSystem(PropertyID id, ISystem* system) { m_Systems[id] = system; }
		inline void RegisterFunction(PropertyID id, IFunction* func) { m_GlobalFunctions[id] = func; }
		inline void RegisterEntityVTable(PropertyID id, std::shared_ptr<EntityVTable> entity) { m_EntityVTables[id] = entity; }
		inline void RegisterArchetype(PropertyID id, std::shared_ptr<ArchetypeVTable> archetype) { m_ArchetypeVTables[id] = archetype; }
		inline void RegisterInterface(PropertyID id, std::shared_ptr<InterfaceVTable> interface) { m_InterfaceVtables[id] = interface; }

		/* Threads ----- */
		inline void FinishBuild() { if (m_BuildThread.joinable()) m_BuildThread.join(); }
		inline void FinishMain() { if (m_MainThread.joinable()) m_MainThread.join(); }

		/* Generation ----- */
		// tokenizeString()
		// BuildTokens()
		// StreamlineTokenChain()
		
		/* Code Running ----- */
		// RunTokens()
		// EvaluateExpression()
		// PushScope()
		// PopScope()


	private:
		EntityID CreateUniqueEntityID() const;

		std::vector<PropertyID> m_Projects;                                    // stores project names to prevent duplicate definitions
		std::map<PropertyID, std::unique_ptr<ComponentPool>> m_ComponentPools; // stores data pools for every component
		std::map<PropertyID, ISystem*> m_Systems;                              // all systems
		std::map<PropertyID, IFunction*> m_GlobalFunctions;                    // globally accessible functions

		std::map<PropertyID, std::shared_ptr<EntityVTable>> m_EntityVTables;       // VTables for Entities
		std::map<PropertyID, std::shared_ptr<ArchetypeVTable>> m_ArchetypeVTables; // VTables for archetypes
		std::map<PropertyID, std::shared_ptr<InterfaceVTable>> m_InterfaceVtables; // VTables for Interfaces

		std::map<EntityID, PropertyID> m_Entities; // all entities and their vtables

		std::thread m_MainThread;  // thread used to run code
		std::thread m_BuildThread; // thread used to build code

		static state* s_Instance;
	};

}