#include "InstructionNodes.h"

#include "../../BismuthState.h"

namespace bismuth::runtime::nodes
{

	void 
	Node_LoadComponentData::Operation(Scope*& scope)
	{
		state* const _State = state::GetGlobal();
		EntityID ent = (m_Location == DataLoadLocation::LoadedInput) ? *static_cast<const EntityID*>(scope->GetIO()->PopData().Data) : scope->GetThis();
		DataPtr ptr = _State->GetComponentPool(m_ComponentID)->GetProperty(ent, m_ItemID)->GetPointer();
		scope->GetIO()->PushData(ptr);
	}

	void Node_SetComponentData::Operation(Scope*& scope)
	{
		state* const _state = state::GetGlobal();
		EntityID ent = (m_Location == DataLoadLocation::LoadedInput) ? *static_cast<const EntityID*>(scope->GetIO()->PopData().Data) : scope->GetThis();
		DataPtr val = scope->GetIO()->PopData();
		_state->GetComponentPool(m_ComponentID)->SetProperty(ent, m_ItemID, val);
	}

	void 
	Node_CreateEntity::Operation(Scope*& scope)
	{
		EntityID entID = state::GetGlobal()->CreateEntity(m_EntityType);
		scope->GetIO()->PushData({ &entID, DataRegistry::GetHashValue("Entity") });
	}

	void Node_DeleteEntity::Operation(Scope*& scope)
	{
		EntityID ent = *static_cast<const EntityID*>(scope->GetIO()->PopData().Data);
		state::GetGlobal()->DestroyEntity(ent);
	}


}