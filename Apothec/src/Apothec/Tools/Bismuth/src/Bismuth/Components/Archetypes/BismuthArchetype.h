#pragma once

#include "../../Data/types.h"
#include "../../Systems/BismuthFunctions.h"

#include <vector>
#include <map>

namespace bismuth
{

	class ArchetypeVTable
	{
	public:
		ArchetypeVTable(std::vector<PropertyID> components, std::map<PropertyID, IFunction*> functions)
			: m_Components(components)
			, m_Functions(functions)
		{}
	
		inline IFunction* const GetFunction(PropertyID id) const { return m_Functions.at(id); }

		inline const std::vector<PropertyID> GetComponents() const { return m_Components; }

	private:
		const std::vector<PropertyID> m_Components;
		std::map<PropertyID, IFunction*> m_Functions;
	};

}