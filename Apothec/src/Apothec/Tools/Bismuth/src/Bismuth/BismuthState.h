#pragma once

#include "Data/types.h"
#include "Components/BismuthComponent.h"
#include "Components/BismuthComponentPool.h"
#include "Systems/BismuthFunctions.h"
#include "Systems/BismuthSystem.h"
#include "Entities/BismuthEntity.h"
#include "Generation/Tokenizer.h"
#include "Data/DataIOStream.h"

#include <thread>

namespace bismuth
{

	class state
	{
	public:
		explicit state();
		~state();
		inline static state* GetGlobal() { return s_Instance; }

		/* Entities ----- */
		EntityID CreateEntity(PropertyID vtable);
		void DestroyEntity(EntityID entity);

		/* Native control ----- */
		inline void RegisterComponent(PropertyID id, ComponentVTable component) { m_ComponentPools[id] = new ComponentPool( component ); }
		inline void RegisterSystem(PropertyID id, ISystem* system) { m_Systems[id] = system; }
		inline void RegisterFunction(PropertyID id, IFunction* func) { m_GlobalFunctions[id] = func; }
		inline void RegisterEntityVTable(PropertyID id, EntityVTable* entity) { m_EntityVTables[id] = entity; }
		inline void RegisterArchetype(PropertyID id, ArchetypeVTable* archetype) { m_ArchetypeVTables[id] = archetype; }
		inline void RegisterInterface(PropertyID id, InterfaceVTable* interface) { m_InterfaceVtables[id] = interface; }

		/* Threads ----- */
		inline void FinishBuild() { if (m_BuildThread.joinable()) m_BuildThread.join(); }
		inline void FinishMain() { if (m_MainThread.joinable()) m_MainThread.join(); }

		/* Generation ----- */
		void BuildFile(const std::string& filePath);
		void BuildTokens(generation::Tokenizer& tokens);
		std::pair<PropertyID, IFunction*> ParseFunction(const std::vector<generation::Token>& tokens, size_t& index);
		// GenerateByteCode()
		
		/* Code Running ----- */
		// RunByteCode()
		// EvaluateExpression()
		// PushScope()
		// PopScope()


	private:
		EntityID CreateUniqueEntityID() const;

		std::vector<PropertyID> m_Projects;                                    // stores project names to prevent duplicate definitions
		std::map<PropertyID, ComponentPool*> m_ComponentPools;                 // stores data pools for every component
		std::map<PropertyID, ISystem*> m_Systems;                              // all systems
		std::map<PropertyID, IFunction*> m_GlobalFunctions;                    // globally accessible functions

		std::map<PropertyID, EntityVTable*> m_EntityVTables;       // VTables for Entities
		std::map<PropertyID, ArchetypeVTable*> m_ArchetypeVTables; // VTables for archetypes
		std::map<PropertyID, InterfaceVTable*> m_InterfaceVtables; // VTables for Interfaces

		std::map<EntityID, PropertyID> m_Entities; // all entities and their vtables

		std::thread m_MainThread;  // thread used to run code
		std::vector<std::string> m_BuildQueue; // files to build
		std::thread m_BuildThread; // threads used to build code

		std::shared_ptr<DataIO> m_IOStream; // iostream for data

		static state* s_Instance;
	};

}