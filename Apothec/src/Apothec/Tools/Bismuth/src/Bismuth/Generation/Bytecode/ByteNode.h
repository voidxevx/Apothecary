#pragma once

namespace bismuth
{
	class state;
}

namespace bismuth::generation::bytecode
{

	/*
	 * Byte chain call Evaluate then ToNext.
	 * evaluate does the operation tied to said node. 
	 * to next sets the code pointer to whatever operation is next (branches will have multiple options to jump to.)
	 */
	class IByteNode
	{
	public:
		virtual ~IByteNode() = default;

		virtual void ToNext(IByteNode*& codePointer) = 0;
		virtual void Evaluate() = 0;

	protected:
		IByteNode(state* _state)
			: m_State(_state)
		{}

		state* m_State;
	};

	/*
	 * Operation Nodes do single operations to alter the state using unput and output streams to pass data.
	 */
	class IByteNode_Operation : public IByteNode
	{
	public:
		virtual ~IByteNode_Operation() = default;

		virtual void ToNext(IByteNode*& codePointer) override final { codePointer = m_Next; }

	protected:
		IByteNode_Operation(state* _state, IByteNode* next)
			: IByteNode(_state)
			, m_Next(next)
		{}

	private:
		IByteNode* m_Next;
	};

	/*
	 * Branch Nodes compare values in the input stream and jump to another node accordingly.
	 * Evaluate should be overriden to change the jump variable
	 */
	class IByteNode_Branch : public IByteNode
	{
	public:
		virtual ~IByteNode_Branch() = default;

		virtual void ToNext(IByteNode*& codePointer) override final { if (m_Jump) codePointer = m_Alternate; else codePointer = m_Next; }

	protected:
		IByteNode_Branch(state* _state, IByteNode* next, IByteNode* alt)
			: IByteNode(_state)
			, m_Next(next)
			, m_Alternate(alt)
		{}

		bool m_Jump = false;

	private:
		IByteNode* m_Next;
		IByteNode* m_Alternate;
	};

}