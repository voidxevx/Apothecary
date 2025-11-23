#pragma once
#include "../ByteNode.h"

/*
	TODO: Make IOstream be a normal return stack - having two different stacks causes to many issues.
*/

namespace bismuth::runtime::nodes
{

	/*
	 * Pushes data from an immediate value into the stream 
	 */
	class Node_ImmediateDataInstance : INode_Operation
	{
	public:
		Node_ImmediateDataInstance(INode* next, const IDataInstance* const data)
			: INode_Operation(next)
			, m_Instance(data)
		{}

		virtual void 
		Operation(DataIO& stream)
		override final
		{
			stream.PushData(m_Instance->GetPointer());
		}

	private:
		const IDataInstance* const m_Instance;
	};

	enum class DataLoadLocation
	{
		LoadedInput,
		ScopeThis,
	};

	/*
	 * Loads the data from an entities component 
	 */
	class Node_LoadComponentData : public INode_Operation
	{
	public:
		Node_LoadComponentData(INode* next, DataLoadLocation accessLoc, PropertyID component, PropertyID item)
			: INode_Operation(next)
			, m_Location(accessLoc)
			, m_ComponentID(component)
			, m_ItemID(item)
		{}

		virtual void Operation(DataIO& stream) override final;

	private:
		const DataLoadLocation m_Location;
		const PropertyID m_ComponentID;
		const PropertyID m_ItemID;
	};

	/*
	 * Creates an entity from a vtable 
	 */
	class Node_CreateEntity : public INode_Operation
	{
	public:
		Node_CreateEntity(INode* next, PropertyID vtable)
			: INode_Operation(next)
			, m_EntityType(vtable)
		{}

		virtual void Operation(DataIO& stream) override final;

	private:
		const PropertyID m_EntityType;
	};

	/*
	 * Deletes an entity 
	 */
	class Node_DeleteEntity : public INode_Operation
	{
	public:
		Node_DeleteEntity(INode* next)
			: INode_Operation(next)
		{}

		virtual void Operation(DataIO& stream) override final;
	};


}