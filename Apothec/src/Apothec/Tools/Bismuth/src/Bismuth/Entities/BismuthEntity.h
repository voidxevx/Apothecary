#pragma once

#include "../Components/Archetypes/BismuthArchetype.h"
#include "../Components/Interfaces/BismuthInterface.h"

namespace bismuth
{

	class EntityVTable
	{
	public:
		EntityVTable(std::vector<PropertyID> components, std::vector<PropertyID> archetypes, std::vector<PropertyID> systems, std::vector<PropertyID> interfaces, std::map<PropertyID, IFunction*> implements, std::vector<IFunction*> constructors)
			: m_Components(components)
			, m_Archetypes(archetypes)
			, m_Systems(systems)
			, m_Interfaces(interfaces)
			, m_Implements(implements)
			, m_Constructors(constructors)
		{}

		inline const std::vector<PropertyID> GetComponents() const { return m_Components; }
		inline const std::vector<PropertyID> GetArchetypes() const { return m_Archetypes; }
		inline const std::vector<PropertyID> GetSystems() const { return m_Systems; }
		inline const std::vector<PropertyID> GetInterfaces() const { return m_Interfaces; }
		inline const std::map<PropertyID, IFunction*> GetImplements() const { return m_Implements; }
		inline const std::vector<IFunction*> GetConstructors() const { return m_Constructors; }

	private:
		const std::vector<PropertyID> m_Components;
		const std::vector<PropertyID> m_Archetypes;
		const std::vector<PropertyID> m_Systems;
		const std::vector<PropertyID> m_Interfaces;
		const std::map<PropertyID, IFunction*> m_Implements;
		const std::vector<IFunction*> m_Constructors;
	};

}