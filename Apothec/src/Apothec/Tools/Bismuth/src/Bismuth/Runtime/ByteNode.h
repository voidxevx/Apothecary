#pragma once

#include <memory>
#include "../Data/DataIOStream.h"

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
		Execute(DataIO& stream)
		{
			Operation(stream);
			Next(stream);
		}

		/*
		 * The functionality of the node.
		 */
		virtual void Operation(DataIO& stream) = 0;
		/*
		 * Call the next node in the sequence. 
		 */
		virtual void Next(DataIO& stream) = 0;
	};



	/*
	 * Operation nodes do one operation then immediately go to the next node. 
	 */
	class INode_Operation : public INode
	{
	public:

		/*
		 * Calls execute for the next node 
		 */
		virtual void 
		Next(DataIO& stream) 
		override final
		{
			if (m_Next)
				m_Next->Execute(stream);
		}

	protected:
		INode_Operation(INode* const next)
			: m_Next(next)
		{}

	private:
		INode* const m_Next;
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
		Next(DataIO& stream) 
		override final
		{
			if (condition)
				m_AlternateNode->Execute(stream);
			else
				m_DefaultNode->Execute(stream);
		}

	protected:
		INode_Branch(INode* const _default, INode* const alt)
			: m_DefaultNode(_default)
			, m_AlternateNode(alt)
		{}

	private:
		bool condition = false;
		INode* const m_DefaultNode;
		INode* const m_AlternateNode;
	};

}