#pragma once

#include "../Data/types.h"
#include "../Data/BismuthDataType.h"
#include "../Systems/BismuthFunctions.h"

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
		Construct(std::vector<IData*>& pool, EntityID entity)
		{
			pool.push_back(new BismuthData<unsigned long long>(entity)); // push a pointer to the entity
			for (const auto& prop : m_Properties)
			{
				pool.push_back(NullDeclData(prop));
			}
		}

		const size_t GetAlignment() const { return m_Alignment; }

		inline IData* const 
		GetProperty(const std::vector<IData*>& pool, size_t componentPtr, PropertyID id)
		const
		{
			return pool[componentPtr + m_PropertyOffsets.at(id)];
		}

		inline void
		SetProperty(std::vector<IData*>& pool, size_t componentPtr, PropertyID id, DataPointer newVal)
		{
			pool[componentPtr + m_PropertyOffsets[id]]->Set(newVal);
		}

		inline IFunction* GetMethod(PropertyID id) const { return m_Methods.at(id); }

	private:
		std::map<PropertyID, size_t> m_PropertyOffsets;
		std::map<PropertyID, IFunction*> m_Methods;
		std::vector<DataTypes> m_Properties;
		size_t m_Alignment;
	};

}