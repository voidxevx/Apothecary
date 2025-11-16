#pragma once

#include "../../Data/types.h"

#include <vector>

namespace bismuth
{

	class InterfaceVTable
	{
	public:
		InterfaceVTable(std::vector<PropertyID> components, std::vector<PropertyID> methods)
			: m_RequiredComponents(components)
			, m_Methods(methods)
		{}

		inline std::vector<PropertyID> GetComponents() const { return m_RequiredComponents; }
		inline std::vector<PropertyID> GetMethods() const { return m_Methods; }

	private:
		std::vector<PropertyID> m_RequiredComponents;
		std::vector<PropertyID> m_Methods;
	};

}