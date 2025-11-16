#pragma once

#include "BismuthFunctions.h"
#include "../Components/Archetypes/BismuthArchetype.h"
#include <vector>
#include <functional>

namespace bismuth
{

	class ISystem
	{
	public:
		virtual ~ISystem() = default;

		void 
		UpdateSystem()
		{
			for (const auto& ent : m_Entities)
				OnUpdate(ent);
		}

		inline void RegisterEntity(EntityID entity) { m_Entities.push_back(entity); }

		inline void 
		RemoveEntity(EntityID entity)
		{
			m_Entities.erase(std::remove(m_Entities.begin(), m_Entities.end(), entity));
		}

		virtual void OnUpdate(EntityID entity) const = 0;

	protected:
		ISystem(std::vector<PropertyID> components)
		{}

	private:
		std::shared_ptr<ArchetypeVTable> m_Archetype;
		std::vector<EntityID> m_Entities;
	};

	class NativeSystem : public ISystem
	{
	public:
		NativeSystem(std::vector<PropertyID> components, std::function<void(EntityID)> function)
			: ISystem(components)
			, m_Lambda(function)
		{}

		virtual void
		OnUpdate(EntityID entity)
		const override final
		{
			m_Lambda(entity);
		}

	private:
		std::function<void(EntityID)> m_Lambda;
	};

	// TODO: Local Systems

}