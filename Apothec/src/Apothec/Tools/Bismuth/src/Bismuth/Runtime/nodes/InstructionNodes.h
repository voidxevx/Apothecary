#pragma once
/*
	Instruction Nodes: 
		* Load immediate data i.e 12, 30, "hello, world", (enums), 4.2
		* Load component data
		* Set component data
		* Create Entity
		* Delete Entity
		* CreateVariable
		* LoadVariable
		* SetVariable
		
		TODO:
		- Data types need operation functions
		* OperationAdd
		* OperationSub
		* OperationMul
		* OperationDiv
		* OperationMod
*/


#include "../ByteNode.h"


/*
	TODO: Make IOstream be a normal return stack - having two different stacks causes to many issues. DONE: 11/23/25
		Context ^ : I had originally had two different stacks that would be for inputs and outputs but i realized that if i tried to store an output from a previous subroutine it would be deallocated.
*/


namespace bismuth::runtime::nodes
{
	enum class DataLoadLocation
	{
		LoadedInput,
		ScopeThis,
	};

	/*
	 * Marks the start of a node chain
	 */
	class Node_Start : public INode_Instruction
	{
	public:
		Node_Start()
		{}

		virtual void Operation(Scope*& scope) override final {}
	};

	class Node_End : public INode_Instruction
	{
	public:
		Node_End()
		{}

		virtual void Operation(Scope*& scope) override final {}
	};

	/*
	 * Pushes data from an immediate value into the stream 
	 */
	class Node_ImmediateDataInstance : public INode_Instruction
	{
	public:
		Node_ImmediateDataInstance(const IDataInstance* data)
			: m_Instance(data)
		{
		}

		~Node_ImmediateDataInstance()
		{
			delete m_Instance;
			m_Instance = nullptr;
		}

		virtual void
		Operation(Scope*& scope)
		override final
		{
			scope->GetIO()->PushData(m_Instance->GetPointer());
		}

	private:
		const IDataInstance* m_Instance;
	};


	/*
	 * Loads the data from an entities component 
	 */
	class Node_LoadComponentData : public INode_Instruction
	{
	public:
		Node_LoadComponentData(DataLoadLocation accessLoc, PropertyID component, PropertyID item)
			: m_Location(accessLoc)
			, m_ComponentID(component)
			, m_ItemID(item)
		{}

		virtual void Operation(Scope*& scope) override final;

	private:
		const DataLoadLocation m_Location;
		const PropertyID m_ComponentID;
		const PropertyID m_ItemID;
	};

	/*
	 * Sets the value of a component variable owned by an entity 
	 */
	class Node_SetComponentData : public INode_Instruction
	{
	public:
		Node_SetComponentData(DataLoadLocation accessLoc, PropertyID component, PropertyID item)
			: m_Location(accessLoc)
			, m_ComponentID(component)
			, m_ItemID(item)
		{}

		virtual void Operation(Scope*& scope) override final;

	private:
		const DataLoadLocation m_Location;
		const PropertyID m_ComponentID;
		const PropertyID m_ItemID;
	};

	/*
	 * Creates an entity from a vtable 
	 */
	class Node_CreateEntity : public INode_Instruction
	{
	public:
		Node_CreateEntity(PropertyID vtable)
			: m_EntityType(vtable)
		{}

		virtual void Operation(Scope*& scope) override final;

	private:
		const PropertyID m_EntityType;
	};

	/*
	 * Deletes an entity 
	 */
	class Node_DeleteEntity : public INode_Instruction
	{
	public:
		Node_DeleteEntity()
		{}

		virtual void Operation(Scope*& scope) override final;
	};

	/*
	 * Creates a variable in the current scope using the last pushed data as its value
	 */
	class Node_CreateVariable : public INode_Instruction
	{
	public:
		Node_CreateVariable(PropertyID id)
			: m_ID(id)
		{}

		virtual void
		Operation(Scope*& scope)
		override final
		{
			scope->CreateVariable(m_ID, scope->GetIO()->PopData());
		}

	private:
		PropertyID m_ID;
	};

	/*
	 * Pushes the value of a variable
	 */
	class Node_GetVariable : public INode_Instruction
	{
	public:
		Node_GetVariable(PropertyID id)
			: m_ID(id)
		{}

		virtual void
		Operation(Scope*& scope)
		override final
		{
			scope->GetIO()->PushData(scope->GetVariable(m_ID));
		}

	private:
		PropertyID m_ID;
	};

	/*
	 * Changes the value of a variable 
	 */
	class Node_SetVariable : public INode_Instruction
	{
	public:
		Node_SetVariable(PropertyID id)
			: m_ID(id)
		{}

		virtual void
		Operation(Scope*& scope)
		override final
		{
			scope->SetVariable(m_ID, scope->GetIO()->PopData());
		}

	private:
		PropertyID m_ID;
	};

}