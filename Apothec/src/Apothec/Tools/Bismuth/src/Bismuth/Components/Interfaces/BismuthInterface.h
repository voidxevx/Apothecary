#pragma once

#include "../../Data/types.h"
#include "../../Systems/BismuthFunctions.h"

#include <vector>

namespace bismuth
{

	struct FunctionImplementation
	{
		PropertyID id;
		PropertyID returnType;
		std::vector<PropertyTemplate> inputs;
	};

	class InterfaceVTable
	{
	public:
		InterfaceVTable(std::vector<PropertyID> components, std::vector<FunctionImplementation> methods)
			: m_RequiredComponents(components)
			, m_Methods(methods)
		{}

		inline std::vector<PropertyID> GetComponents() const { return m_RequiredComponents; }
		inline std::vector<FunctionImplementation> GetMethods() const { return m_Methods; }

	private:
		std::vector<PropertyID> m_RequiredComponents;
		std::vector<FunctionImplementation> m_Methods;
	};

}