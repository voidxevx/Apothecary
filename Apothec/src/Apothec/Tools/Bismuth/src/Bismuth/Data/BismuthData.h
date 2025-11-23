#pragma once

#include <map>
#include <string>

#include "types.h"

namespace bismuth
{
	typedef unsigned long long TypeID;

	/*
	 * Data pointers simple point to the data of an instance they cannot edit the actual value.
	 * Primary use is to have an analogous reference that can easily be casted to another type.
	 */
	struct DataPtr
	{
		const void* Data;
		TypeID Type;

		DataPtr(const void* const value, TypeID type)
			: Data(value)
			, Type(type)
		{}

		void
		operator=(const DataPtr& other)
		{
			Data = other.Data;
			Type = other.Type;
		}
	};

	/*
	 * Data instances are what actually store data for a specific type.
	 */
	class IDataInstance
	{
	public:
		virtual ~IDataInstance()
		{
			delete m_Ptr->Data;
			m_Ptr->Data = nullptr;
			delete m_Ptr;
			m_Ptr = nullptr;
		}

		inline const DataPtr GetPointer() const { return *m_Ptr; }
		virtual void SetValue(const DataPtr&) = 0;

	protected:
		template<typename _T>
		IDataInstance(TypeID id, _T val)
			: m_ID(id)
		{
			m_Ptr = new DataPtr{ &val, id };
		}

		TypeID m_ID;
		DataPtr* m_Ptr;
	};


	/*
	 * This is essentially a blueprint for how the data type will be casted and edited at a static level.
	 */
	class IDataType
	{
		friend class DataRegistry;
	public:
		virtual DataPtr CastTo(const DataPtr& val) const { return { nullptr, 0 }; }
		virtual inline IDataInstance* NullDecl() const { return nullptr; }

	protected:
		IDataType(const std::string& alias);

		TypeID m_ID = 0;
	};

	/*
	 * Holds references to an instance of every data types interface.
	 */
	class DataRegistry
	{
	public:
		explicit DataRegistry()
		{
			s_Instance = this;
		}

		void 
		RegisterDataType(const std::string& alias, IDataType* data)
		{
			TypeID id = s_DataHasher(alias);
			m_DataTypes[id] = data;
			data->m_ID = id;
		}

		DataPtr SmartCast(const DataPtr& data, TypeID to) const
		{
			if (m_DataTypes.count(to) > 0)
				return m_DataTypes.at(to)->CastTo(data);
			else
				return { nullptr, 0 };
		}

		inline IDataInstance* 
		NullDeclType(TypeID id) 
		const
		{
			if (m_DataTypes.count(id) > 0)
				return m_DataTypes.at(id)->NullDecl();
			else
				return nullptr;
		}

		static inline DataRegistry* Get() { return s_Instance; }
		static inline std::hash<std::string> GetHasher() { return s_DataHasher; }

		static TypeID GetHashValue(const std::string& value) { return s_DataHasher(value); }

	private:
		std::map<TypeID, IDataType*> m_DataTypes;

		static DataRegistry* s_Instance;
		static std::hash<std::string> s_DataHasher;
	};



	#define DATAINSTANCEOVERRIDE(t) virtual void SetValue(const DataPtr& ptr) override final { if (ptr.Type == m_ID) data = *static_cast<const t*>(ptr.Data); }


	class IntDataInstance : public IDataInstance
	{
	public:
		IntDataInstance(TypeID id, int value)
			: data(value)
			, IDataInstance(id, data)
		{}

		DATAINSTANCEOVERRIDE(int)

	private:
		int data;
	};


	class IntDataType : public IDataType
	{
	public:
		IntDataType()
			: IDataType("int32")
		{}

		virtual DataPtr
		CastTo(const DataPtr& val)
		const
		{
			if (DataRegistry::GetHashValue("int"))
				return val;
			else if (DataRegistry::GetHashValue("str"))
			{
				try
				{
					int t_val = std::stoi(*static_cast<const std::string*>(val.Data));
					return { &t_val, m_ID };
				}
				catch (...)
				{
					return { new int{0}, m_ID };
				}

			}
			else
				return { nullptr, 0 };
		}

		virtual inline IDataInstance*
		NullDecl()
		const override final
		{
			return new IntDataInstance(m_ID, 0);
		}

	};


	class StringDataInstance : public IDataInstance
	{
	public:
		StringDataInstance(TypeID id, std::string value)
			: data(value)
			, IDataInstance(id, value)
		{}

		DATAINSTANCEOVERRIDE(std::string)

	private:
		std::string data;
	};

	class StringDataType : public IDataType
	{
	public:
		StringDataType()
			: IDataType("str")
		{}

		virtual DataPtr
		CastTo(const DataPtr& val)
		const
		{
			if (val.Type == DataRegistry::GetHashValue("str"))
				return val;
			else if (val.Type == DataRegistry::GetHashValue("int32"))
				return { new std::string(std::to_string(*static_cast<const int* const>(val.Data))), m_ID };
			else
				return { new std::string(""), m_ID };
		}

		virtual inline IDataInstance*
		NullDecl()
		const override final
		{
			return new StringDataInstance{ m_ID, "" };
		}
	};

	class EntityPtrDataInstance : public IDataInstance
	{
	public:
		EntityPtrDataInstance(TypeID id, EntityID value)
			: data(value)
			, IDataInstance(id, data)
		{}

		DATAINSTANCEOVERRIDE(EntityID)

	private:
		EntityID data;
	};

	class EntityPtrDataType : public IDataType
	{
	public:
		EntityPtrDataType()
			: IDataType("Entity")
		{}

		virtual DataPtr
		CastTo(const DataPtr& val)
		const
		{
			if (val.Type == m_ID)
				return val;
			else
				return { nullptr, 0 };
		}

		virtual inline IDataInstance*
		NullDecl()
		const override final
		{
			return new EntityPtrDataInstance(m_ID, 0);
		}
	};


}