/*
	Nodes are essentially bytecode instructions stored as objects.
	The nodes are interpreted in sequence dictated by the linked list structure that they create.
	There are two different types of linkage: Operational -> continue to the next node after completing an operation.
										  and Branching -> select a different node depending on a condition.
*/

#pragma once

#include "BismuthScope.h"

namespace bismuth::runtime
{

	/*
	 * Nodes represent individual instructions for the "vm" 
	 */
	class INode
	{
	public:

		/*
		 * Execute automatically calls the functionality of the node and then calls the next node.
		 */
		void
		Execute(Scope*& stream)
		{
			Operation(stream);
			Next(stream);
		}

		/*
		 * The functionality of the node.
		 */
		virtual void Operation(Scope*& stream) = 0;
		/*
		 * Call the next node in the sequence. 
		 */
		virtual void Next(Scope*& stream) = 0;

		/*
		 * Interface to change set the next node 
		 */
		virtual void SetNext(INode* next) = 0;

		/*
		 * Interface to change the alternate next node (for branch nodes only) 
		 */
		virtual void SetAlternate(INode* next) = 0;
	};



	/*
	 * Instruction nodes do one operation then immediately go to the next node. 
	 */
	class INode_Instruction : public INode
	{
	public:

		/*
		 * Calls execute for the next node 
		 */
		virtual void 
		Next(Scope*& stream) 
		override final
		{
			if (m_Next)
				m_Next->Execute(stream);
		}

		virtual void SetNext(INode* next) override final { m_Next = next; }
		virtual void SetAlternate(INode*) override final {}

	private:
		INode* m_Next = nullptr;
	};

	/*
	 * Branch nodes jump optionally jump to a different node it the condition is changed during the operation function. 
	 */
	class INode_Branch : public INode
	{
	public:

		/*
		 *  executes the alternate node if condition is true
		 */
		virtual void 
		Next(Scope*& stream) 
		override final
		{
			if (condition)
				m_AlternateNode->Execute(stream);
			else
				m_DefaultNode->Execute(stream);
		}

		virtual void SetNext(INode* next) override final { m_DefaultNode = next; }
		virtual void SetAlternate(INode* alt) override final { m_AlternateNode = alt; }

	protected:
		INode_Branch(INode* const _default, INode* const alt)
			: m_DefaultNode(_default)
			, m_AlternateNode(alt)
		{}

	private:
		bool condition = false;
		INode* m_DefaultNode = nullptr;
		INode* m_AlternateNode = nullptr;
	};

}