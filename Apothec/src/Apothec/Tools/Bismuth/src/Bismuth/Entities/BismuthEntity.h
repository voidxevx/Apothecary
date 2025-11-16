#pragma once

#include "../Components/Archetypes/BismuthArchetype.h"
#include "../Components/Interfaces/BismuthInterface.h"

namespace bismuth
{

	class EntityVTable
	{
	public:
		EntityVTable(std::vector<PropertyID> components, std::vector<PropertyID> archetypes, std::vector<PropertyID> systems, std::vector<PropertyID> interfaces, std::vector<IFunction*> implements)
			: m_Components(components)
			, m_Archetypes(archetypes)
			, m_Systems(systems)
			, m_Interfaces(interfaces)
			, m_Implements(implements)
		{}

		inline const std::vector<PropertyID> GetComponents() const { return m_Components; }
		inline const std::vector<PropertyID> GetArchetypes() const { return m_Archetypes; }
		inline const std::vector<PropertyID> GetSystems() const { return m_Systems; }
		inline const std::vector<PropertyID> GetInterfaces() const { return m_Interfaces; }
		inline const std::vector<IFunction*> GetImplements() const { return m_Implements; }

	private:
		const std::vector<PropertyID> m_Components;
		const std::vector<PropertyID> m_Archetypes;
		const std::vector<PropertyID> m_Systems;
		const std::vector<PropertyID> m_Interfaces;
		const std::vector<IFunction*> m_Implements;
	};

}