#include "BismuthScope.h"

namespace bismuth::runtime
{

	Scope::~Scope()
	{
		for (auto& inst : m_ScopeVariable)
			delete inst.second;

		for (size_t i{}; i < m_TemporaryDataCount; ++i)
		{
			delete m_TemporaryData[i];
			m_TemporaryData[i] = nullptr;
		}

		free(m_TemporaryData);
		m_TemporaryData = nullptr;
	}

	void
	Scope::CreateVariable(PropertyID id, const DataPtr& value)
	{
		IDataInstance* data = DataRegistry::Get()->NullDeclType(value.Type);
		data->SetValue(value);
		m_ScopeVariable[id] = data;
	}

	void
	Scope::SetVariable(PropertyID id, const DataPtr& value)
	{
		m_ScopeVariable[id]->SetValue(value);
	}

	const DataPtr
	Scope::GetVariable(PropertyID id)
	const
	{
		return m_ScopeVariable.at(id)->GetPointer();
	}

	void
	Scope::PopScope(Scope*& scopePointer)
	{
		scopePointer = m_Last;
		delete this;
	}

	void
	Scope::PushScope(Scope*& scopePointer, bool downAccess, EntityID _this)
	{
		Scope* next = new Scope((_this == 0) ? m_ScopeThis : _this, this, downAccess, m_IOStream);
		scopePointer = next;
	}

}