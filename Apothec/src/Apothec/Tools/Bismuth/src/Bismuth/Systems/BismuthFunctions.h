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
		IFunction(DataTypes returnType, std::vector<PropertyTemplate> inputs)
			: m_Inputs(inputs)
		{}

	private:
		std::vector<PropertyTemplate> m_Inputs;
	};

	class NativeFunction : public IFunction
	{
	public:
		NativeFunction(DataTypes returnType, std::vector<PropertyTemplate> inputs, std::function<int()> func)
			: IFunction(returnType, inputs)
			, m_Lambda(func)
		{}

		inline virtual void Call() { m_Lambda(); }

	private:
		std::function<int()> m_Lambda;
	};

	// TODO: Local functions

}