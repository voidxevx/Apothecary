#pragma once

#include <string>
#include "../../../Argon/src/Argon.h"

namespace bismuth
{

	enum DataTypes : unsigned char
	{
		Int32, Uint32, Int64, Uint64, // Integers
		Float, Double, // Floating point
		Boolean, // booleans
		String, // strings
		Vec2, Vec3, Vec4, // vectors
		Mat2, Mat3, Mat4, Mat2x3, Mat3x2, Mat3x4, Mat4x3, Mat2x4, Mat4x2, // matrices
		EntityRef, // pointer to an entity
		ComponentRef, // pointer to a component
		PrimitiveReferance, // pointer to a datatype
		Custom, // other
	};

	template<typename _T>
	constexpr DataTypes
	GetDataType()
	{
		return DataTypes::Custom;
	}

#define GETDATATYPE(t, v) template<> constexpr DataTypes GetDataType<t>() { return DataTypes::v; }

	using mat2 = argon::Matrix<float, 2, 2>;
	using mat3 = argon::Matrix<float, 3, 3>;
	using mat4 = argon::Matrix<float, 4, 4>;
	using mat2x3 = argon::Matrix<float, 2, 3>;
	using mat3x2 = argon::Matrix<float, 3, 2>;
	using mat3x4 = argon::Matrix<float, 3, 4>;
	using mat4x3 = argon::Matrix<float, 4, 3>;
	using mat2x4 = argon::Matrix<float, 2, 4>;
	using mat4x2 = argon::Matrix<float, 4, 2>;

	GETDATATYPE(int, Int32)
	GETDATATYPE(unsigned int, Uint32)
	GETDATATYPE(long long, Int64)
	GETDATATYPE(unsigned long long, Uint64)
	GETDATATYPE(float, Float)
	GETDATATYPE(double, Double)
	GETDATATYPE(bool, Boolean)
	GETDATATYPE(std::string, String)
	GETDATATYPE(argon::vec2, Vec2)
	GETDATATYPE(argon::vec3, Vec3)
	GETDATATYPE(argon::vec4, Vec4)
	GETDATATYPE(mat2, Mat2)
	GETDATATYPE(mat3, Mat3)
	GETDATATYPE(mat4, Mat4)
	GETDATATYPE(mat2x3, Mat2x3)
	GETDATATYPE(mat3x2, Mat3x2)
	GETDATATYPE(mat3x4, Mat3x4)
	GETDATATYPE(mat4x3, Mat4x3)
	GETDATATYPE(mat2x4, Mat2x4)
	GETDATATYPE(mat4x2, Mat4x2)

	/*
	 * Holds an incomplete pointer to a variable as well as an accurate type hint.
	 */
	struct DataPointer
	{
		const void* const Data;
		DataTypes Type;

		template<typename _T>
		DataPointer(const _T* const data)
			: Data(data)
			, Type(GetDataType<_T>())
		{}

	};

	/*
	 * Automatically casts a data pointer to another using the type hint 
	 */
	template<typename _T>
	extern _T SmartCast(const DataPointer& data);


	/*
	 * Interface Class for Bismuth data variables 
	 */
	class IData
	{
	public:
		virtual ~IData() = default;

		virtual inline DataPointer Get() const = 0;
		virtual inline void Set(const DataPointer& value) = 0;
		
		template<typename _T>
		inline _T
		GetAs()
		const
		{
			return SmartCast<_T>(Get());
		}

	protected:
		IData(DataTypes type)
			: m_Type(type)
		{}

		DataTypes m_Type;
	};



	template<typename _T>
	class BismuthData : public IData
	{
	public:
		BismuthData(_T value)
			: IData(GetDataType<_T>())
			, m_Data(value)
		{}

		virtual ~BismuthData() = default;

		virtual inline DataPointer
		Get()
		const override final
		{
			return DataPointer(&m_Data);
		}

		virtual inline void
		Set(const DataPointer& value)
		override final
		{
			m_Data = SmartCast<_T>(value);
		}

	private:
		_T m_Data;
	};

	inline IData* const 
	NullDeclData(DataTypes type)
	{
		switch (type)
		{
		case DataTypes::Int32:
			return new BismuthData<int>(0);
		case DataTypes::Uint32:
			return new BismuthData<unsigned int>(0);
		case DataTypes::Int64:
			return new BismuthData<long long>(0);
		case DataTypes::Uint64:
			return new BismuthData<unsigned long long>(0);
		case DataTypes::Float:
			return new BismuthData<float>(0);
		case DataTypes::Double:
			return new BismuthData<double>(0);
		case DataTypes::Boolean:
			return new BismuthData<bool>(false);
		case DataTypes::String:
			return new BismuthData<std::string>("");
		default:
			return nullptr;
		}
	}

}