#include "OperationNodes.h"

#include "../../BismuthState.h"

namespace bismuth::runtime::nodes
{

	void 
	Node_LoadComponentData::Operation(DataIO& stream)
	{
		state* const _State = state::GetGlobal();
		EntityID ent = (m_Location == DataLoadLocation::LoadedInput) ? *static_cast<const EntityID*>(stream.PopData().Data) : 0 /* TODO: scope this*/;
		DataPtr ptr = _State->GetComponentPool(m_ComponentID)->GetProperty(ent, m_ItemID)->GetPointer();
		stream.PushData(ptr);
	}

	void 
	Node_CreateEntity::Operation(DataIO& stream)
	{
		EntityID entID = state::GetGlobal()->CreateEntity(m_EntityType);
		stream.PushData({ &entID, DataRegistry::GetHashValue("Entity") });
	}

	void Node_DeleteEntity::Operation(DataIO& stream)
	{
		EntityID ent = *static_cast<const EntityID*>(stream.PopData().Data);
		state::GetGlobal()->DestroyEntity(ent);
	}

}