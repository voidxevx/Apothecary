#pragma once

#include <map>
#include <string>

namespace bismuth
{
	typedef unsigned long long TypeID;

	class DataRegistry
	{
	public:
		DataRegistry()
		{
			s_Instance = this;
		}

		void 
		RegisterDataType(const std::string& alias, IData data)
		{
			TypeID id = s_DataHasher(alias);
			m_DataTypes[id] = data;
			data.m_ID = id;
		}

		DataPointer SmartCast(const DataPointer& data, TypeID to) const
		{
			if (m_DataTypes.count(to) > 0)
				return m_DataTypes.at(to).CastTo(data);
			else
				return { nullptr, 0 };
		}

		static inline DataRegistry* Get() { return s_Instance; }
		static inline std::hash<std::string> GetHasher() { return s_DataHasher; }

		static constexpr TypeID GetHashValue(const std::string& value) { return s_DataHasher(value); }

	private:
		std::map<TypeID, IData> m_DataTypes;

		static DataRegistry* s_Instance;
		static std::hash<std::string> s_DataHasher;
	};

	struct DataPointer
	{
		void* Data;
		TypeID Type;

		DataPointer(void* const value, TypeID type)
			: Data(value)
			, Type(type)
		{}

		~DataPointer()
		{
			delete Data;
		}

		void
		CopyData(DataPointer data)
		{
			delete Data;
			Data = data.Data;
			Type = data.Type;
		}
	};

	class IData
	{
		friend class DataRegistry;
	public:
		virtual DataPointer CastTo(const DataPointer& val) const { return { nullptr, 0 }; }
		virtual inline DataPointer NullDecl() const { return { nullptr, 0 }; }

	protected:
		IData(const std::string& alias)
		{
			DataRegistry::Get()->RegisterDataType(alias, *this);
		}

		TypeID m_ID = 0;
	};

	class IntData : public IData
	{
	public:
		IntData()
			: IData("int32")
		{}

		virtual DataPointer 
		CastTo(const DataPointer& val)
		const
		{
			if (val.Type == DataRegistry::GetHashValue("int"))
				return val;
			else if (val.Type == DataRegistry::GetHashValue("float"))
				return { new int{(int)*static_cast<float*>(val.Data)}, m_ID };
			else 
				return NullDecl();
		}

		virtual inline DataPointer
		NullDecl()
		const
		{
			return { new int{0}, m_ID };
		}

	};

	static IntData intData = IntData();


	class FloatData : public IData
	{
	public:
		FloatData()
			: IData("float")
		{}

		virtual DataPointer 
		CastTo(const DataPointer& val)
		const
		{
			if (val.Type == DataRegistry::GetHashValue("float"))
				return val;
			else if (val.Type == DataRegistry::GetHashValue("int"))
				return { new float{(float)*static_cast<int*>(val.Data)}, m_ID };
		}

		virtual inline DataPointer
		NullDecl()
		const
		{
			return { new int{0}, m_ID };
		}
	};

	static FloatData floatData = FloatData();

}