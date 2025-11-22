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
		Execute()
		{
			Operation();
			Next();
		}

		/*
		 * The functionality of the node.
		 */
		virtual void Operation() = 0;
		/*
		 * Call the next node in the sequence. 
		 */
		virtual void Next() = 0;

	protected:
		INode(std::shared_ptr<DataIO> stream)
			: m_IOStream(stream)
		{}

		std::shared_ptr<DataIO> m_IOStream;
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
		Next() 
		override final
		{
			if (m_Next)
				m_Next->Execute();
		}

	protected:
		INode_Operation(std::shared_ptr<DataIO> stream, INode* const next)
			: INode(stream)
			, m_Next(next)
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
		Next() 
		override final
		{
			if (condition)
				m_AlternateNode->Execute();
			else
				m_DefaultNode->Execute();
		}

	protected:
		INode_Branch(std::shared_ptr<DataIO> stream, INode* const _default, INode* const alt)
			: INode(stream)
			, m_DefaultNode(_default)
			, m_AlternateNode(alt)
		{}

	private:
		bool condition = false;
		INode* const m_DefaultNode;
		INode* const m_AlternateNode;
	};

}