#pragma once

#include "Types.h"

#include <optional>
#include <algorithm>

namespace xenon
{

	template<typename _T>
	struct PoolElement
	{
		_T* item;
		XenonID id;
	};

	class IComponentPool
	{
	public:
		virtual ~IComponentPool() {}
		virtual bool remove(XenonID id) = 0;
		virtual void move(XenonID current, XenonID target) = 0;
 	};

	template<typename _T>
	class ComponentPool : public IComponentPool
	{
	public:
		ComponentPool(XenonID max)
			: m_Max(max)
		{
			m_Sparse = (XenonID*)malloc(sizeof(XenonID) * max);
			m_Dense = (PoolElement<_T>*)malloc(sizeof(PoolElement<_T>) * max);
		}
		virtual ~ComponentPool()
		{
			free(m_Sparse);
			free(m_Dense);
			m_Sparse = nullptr;
			m_Dense = nullptr;
		}

		void add(XenonID id, _T* item)
		{
			m_Sparse[id] = m_Size;
			m_Dense[m_Size] = { item, id };
			++m_Size;
		}

		virtual bool remove(XenonID id) override
		{
			if (!contains(id)) return false;
			--m_Size;
			std::swap(m_Dense[id], m_Dense[m_Size]);
			m_Dense[m_Size].id = m_Max + 1;
			delete m_Dense[m_Size].item;
			m_Dense[m_Size].item = nullptr;
			return true;
		}

		bool contains(XenonID id)
		{
			if (id > m_Max) return false;

			XenonID DenseID = m_Sparse[id];
			if (DenseID > m_Size) return false;

			PoolElement<_T> element = m_Dense[DenseID]; 
			return element.id == id; /* cross reference ids */

		}

		std::optional<_T*> GetComponent(XenonID id)
		{
			if (!contains(id)) return std::nullopt;
			
			return m_Dense[m_Sparse[id]].item;
		}

		virtual void move(XenonID current, XenonID target) override
		{
			m_Dense[m_Sparse[current]].id = target;
			m_Sparse[target] = m_Sparse[current];
		}

		inline const XenonID Size() const { return m_Size; }

	private:
		XenonID m_Size = 0;
		XenonID m_Max;
		XenonID* m_Sparse;
		PoolElement<_T>* m_Dense;
	};

}
