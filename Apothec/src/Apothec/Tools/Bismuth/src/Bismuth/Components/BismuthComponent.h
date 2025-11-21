#pragma once

#include "../Systems/BismuthFunctions.h"
#include "../Data/BismuthData.h"

#include <string>
#include <map>
#include <vector>

namespace bismuth
{

	class ComponentVTable
	{
	public:
		ComponentVTable(std::vector<std::pair<PropertyID, IFunction*>> methods, std::vector<PropertyTemplate> properties)
		{
			size_t offset = 1; // starts at 1 because 0 is a pointer to the entity
			for (const auto& prop : properties)
			{
				//m_Properties.push_back(prop.Type);
				m_PropertyOffsets[prop.ID] = offset;
				++offset;
			}
			m_Alignment = offset;

			for (const auto& method : methods)
			{
				m_Methods[method.first] = method.second;
			}
		}

		void 
		Construct(std::vector<IDataInstance*>& pool, EntityID entity)
		{
			pool.push_back(new EntityPtrDataInstance(DataRegistry::GetHashValue("Entity"), entity)); // push a pointer to the entity
			for (const auto& prop : m_Properties)
			{
				pool.push_back(DataRegistry::Get()->NullDeclType(prop));
			}
		}

		const size_t GetAlignment() const { return m_Alignment; }

		inline IDataInstance* const 
		GetProperty(const std::vector<IDataInstance*>& pool, size_t componentPtr, PropertyID id)
		const
		{
			return pool[componentPtr + m_PropertyOffsets.at(id)];
		}

		inline void
		SetProperty(std::vector<IDataInstance*>& pool, size_t componentPtr, PropertyID id, DataPtr newVal)
		{
			pool[componentPtr + m_PropertyOffsets[id]]->SetValue(newVal);
		}

		inline IFunction* GetMethod(PropertyID id) const { return m_Methods.at(id); }

	private:
		std::map<PropertyID, size_t> m_PropertyOffsets;
		std::map<PropertyID, IFunction*> m_Methods;
		std::vector<TypeID> m_Properties;
		size_t m_Alignment;
	};

}