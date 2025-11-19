#pragma once

#include "../Data/BismuthDataType.h"
#include <vector>
#include <functional>

namespace bismuth
{

	class IFunction
	{
	public:
		virtual ~IFunction() = default;

		virtual void Call() = 0;

		inline const std::vector<PropertyTemplate>& GetInputs() const { return m_Inputs; }

	protected:
		IFunction(PropertyID returnType, std::vector<PropertyTemplate> inputs)
			: m_Inputs(inputs)
			, m_ReturnType(returnType)
		{}

	private:
		std::vector<PropertyTemplate> m_Inputs;
		PropertyID m_ReturnType;
	};

	class NativeFunction : public IFunction
	{
	public:
		NativeFunction(PropertyID returnType, std::vector<PropertyTemplate> inputs, std::function<int()> func)
			: IFunction(returnType, inputs)
			, m_Lambda(func)
		{}

		inline virtual void Call() override final { m_Lambda(); }

	private:
		std::function<int()> m_Lambda;
	};

	class LocalFunction : public IFunction
	{
	public:
		LocalFunction(PropertyID returnType, std::vector<PropertyTemplate> inputs /*byte code*/)
			: IFunction(returnType, inputs)
		{}

		virtual void Call() override final {}

	private:
	};
}