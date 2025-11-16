#include "BismuthDataType.h"

namespace bismuth
{

	static bool 
	is_int(const std::string& str)
	{
		try
		{
			int test = std::stoi(str);
		}
		catch (...)
		{
			return false;
		}
		return true;
	}
	static bool 
	is_long(const std::string& str)
	{
		try
		{
			long long test = std::stoll(str);
		}
		catch (...)
		{
			return false;
		}
		return true;
	}
	static bool 
	is_float(const std::string& str)
	{
		try
		{
			float test = std::stof(str);
		}
		catch (...)
		{
			return false;
		}
		return true;
	}
	static bool 
	is_double(const std::string& str)
	{
		try
		{
			double test = std::stod(str);
		}
		catch (...)
		{
			return false;
		}
		return true;
	}

	template<>
	int 
	SmartCast(const DataPointer& data)
	{
		int ret = 0;

		switch (data.Type)
		{
		case DataTypes::Int32:
			ret = *static_cast<const int*>(data.Data);
			break;
		case DataTypes::Uint32:
			ret = (signed)*static_cast<const unsigned int*>(data.Data);
			break;
		case DataTypes::Int64:
			ret = (int)*static_cast<const long long*>(data.Data);
			break;
		case DataTypes::Uint64:
			ret = (signed int)*static_cast<const unsigned long long*>(data.Data);
			break;
		case DataTypes::Float:
			ret = (int)*static_cast<const float*>(data.Data);
			break;
		case DataTypes::Double:
			ret = (int)*static_cast<const double*>(data.Data);
			break;
		case DataTypes::Boolean:
			ret = (int)*static_cast<const bool*>(data.Data);
			break;
		case DataTypes::String:
			if (is_int(*static_cast<const std::string*>(data.Data)))
			{
				ret = std::stoi(*static_cast<const std::string*>(data.Data));
			}
			break;
		default:
			break;
		}

		return ret;
	}

	template<>
	unsigned int 
	SmartCast(const DataPointer& data)
	{
		unsigned int ret = 0;

		switch (data.Type)
		{
		case DataTypes::Int32:
			ret = (unsigned)*static_cast<const int*>(data.Data);
			break;
		case DataTypes::Uint32:
			ret = *static_cast<const unsigned int*>(data.Data);
			break;
		case DataTypes::Int64:
			ret = (unsigned int)*static_cast<const long long*>(data.Data);
			break;
		case DataTypes::Uint64:
			ret = (unsigned int)*static_cast<const unsigned long long*>(data.Data);
			break;
		case DataTypes::Float:
			ret = (unsigned int)*static_cast<const float*>(data.Data);
			break;
		case DataTypes::Double:
			ret = (unsigned int)*static_cast<const double*>(data.Data);
			break;
		case DataTypes::Boolean:
			ret = (unsigned int)*static_cast<const bool*>(data.Data);
			break;
		case DataTypes::String:
			if (is_int(*static_cast<const std::string*>(data.Data)))
			{
				ret = (unsigned)std::stoi(*static_cast<const std::string*>(data.Data));
			}
			break;
		default:
			break;
		}

		return ret;
	}

	template<>
	long long
	SmartCast(const DataPointer& data)
	{
		long long ret = 0;

		switch (data.Type)
		{
		case DataTypes::Int32:
			ret = (long long)*static_cast<const int*>(data.Data);
			break;
		case DataTypes::Uint32:
			ret = (signed long long)*static_cast<const unsigned int*>(data.Data);
			break;
		case DataTypes::Int64:
			ret = *static_cast<const long long*>(data.Data);
			break;
		case DataTypes::Uint64:
			ret = (signed)*static_cast<const unsigned long long*>(data.Data);
			break;
		case DataTypes::Float:
			ret = (long long)*static_cast<const float*>(data.Data);
			break;
		case DataTypes::Double:
			ret = (long long)*static_cast<const double*>(data.Data);
			break;
		case DataTypes::Boolean:
			ret = (long long)*static_cast<const bool*>(data.Data);
			break;
		case DataTypes::String:
			if (is_long(*static_cast<const std::string*>(data.Data)))
			{
				ret = std::stoll(*static_cast<const std::string*>(data.Data));
			}
			break;
		default:
			break;
		}

		return ret;
	}

	template<>
	unsigned long long
	SmartCast(const DataPointer& data)
	{
		unsigned long long ret = 0;

		switch (data.Type)
		{
		case DataTypes::Int32:
			ret = (unsigned long long)*static_cast<const int*>(data.Data);
			break;
		case DataTypes::Uint32:
			ret = (unsigned long long)*static_cast<const unsigned int*>(data.Data);
			break;
		case DataTypes::Int64:
			ret = (unsigned)*static_cast<const long long*>(data.Data);
			break;
		case DataTypes::Uint64:
			ret = *static_cast<const unsigned long long*>(data.Data);
			break;
		case DataTypes::Float:
			ret = (unsigned long long)*static_cast<const float*>(data.Data);
			break;
		case DataTypes::Double:
			ret = (unsigned long long)*static_cast<const double*>(data.Data);
			break;
		case DataTypes::Boolean:
			ret = (unsigned long long)*static_cast<const bool*>(data.Data);
			break;
		case DataTypes::String:
			if (is_long(*static_cast<const std::string*>(data.Data)))
			{
				ret = (unsigned)std::stoll(*static_cast<const std::string*>(data.Data));
			}
			break;
		default:
			break;
		}

		return ret;
	}

	template<>
	float
	SmartCast(const DataPointer& data)
	{
		float ret = 0;

		switch (data.Type)
		{
		case DataTypes::Int32:
			ret = (float)*static_cast<const int*>(data.Data);
			break;
		case DataTypes::Uint32:
			ret = (float)*static_cast<const unsigned int*>(data.Data);
			break;
		case DataTypes::Int64:
			ret = (float)*static_cast<const long long*>(data.Data);
			break;
		case DataTypes::Uint64:
			ret = (float)*static_cast<const unsigned long long*>(data.Data);
			break;
		case DataTypes::Float:
			ret = *static_cast<const float*>(data.Data);
			break;
		case DataTypes::Double:
			ret = (float)*static_cast<const double*>(data.Data);
			break;
		case DataTypes::Boolean:
			ret = (float)*static_cast<const bool*>(data.Data);
			break;
		case DataTypes::String:
			if (is_float(*static_cast<const std::string*>(data.Data)))
			{
				ret = std::stof(*static_cast<const std::string*>(data.Data));
			}
			break;
		default:
			break;
		}

		return ret;
	}

	template<>
	double
	SmartCast(const DataPointer& data)
	{
		double ret = 0;

		switch (data.Type)
		{
		case DataTypes::Int32:
			ret = (double)*static_cast<const int*>(data.Data);
			break;
		case DataTypes::Uint32:
			ret = (double)*static_cast<const unsigned int*>(data.Data);
			break;
		case DataTypes::Int64:
			ret = (double)*static_cast<const long long*>(data.Data);
			break;
		case DataTypes::Uint64:
			ret = (double)*static_cast<const unsigned long long*>(data.Data);
			break;
		case DataTypes::Float:
			ret = (double)*static_cast<const float*>(data.Data);
			break;
		case DataTypes::Double:
			ret = *static_cast<const double*>(data.Data);
			break;
		case DataTypes::Boolean:
			ret = (double)*static_cast<const bool*>(data.Data);
			break;
		case DataTypes::String:
			if (is_double(*static_cast<const std::string*>(data.Data)))
			{
				ret = std::stod(*static_cast<const std::string*>(data.Data));
			}
			break;
		default:
			break;
		}

		return ret;
	}

	template<>
	bool
	SmartCast(const DataPointer& data)
	{
		bool ret = false;

		switch (data.Type)
		{
		case DataTypes::Int32:
			ret = *static_cast<const int*>(data.Data) != 0;
			break;
		case DataTypes::Uint32:
			ret = *static_cast<const unsigned int*>(data.Data) != 0;
			break;
		case DataTypes::Int64:
			ret = *static_cast<const long long*>(data.Data) != 0;
			break;
		case DataTypes::Uint64:
			ret = *static_cast<const unsigned long long*>(data.Data) != 0;
			break;
		case DataTypes::Float:
			ret = *static_cast<const float*>(data.Data) != 0;
			break;
		case DataTypes::Double:
			ret = *static_cast<const double*>(data.Data) != 0;
			break;
		case DataTypes::Boolean:
			ret = *static_cast<const bool*>(data.Data);
			break;
		case DataTypes::String:
			if (*static_cast<const std::string*>(data.Data) == "true")
			{
				ret = true;
			}
			break;
		default:
			break;
		}

		return ret;
	}

	template<>
	std::string
	SmartCast(const DataPointer& data)
	{
		std::string ret = "NULL";

		switch (data.Type)
		{
		case DataTypes::Int32:
			ret = std::to_string(*static_cast<const int*>(data.Data));
			break;
		case DataTypes::Uint32:
			ret = std::to_string(*static_cast<const unsigned int*>(data.Data));
			break;
		case DataTypes::Int64:
			ret = std::to_string(*static_cast<const long long*>(data.Data));
			break;
		case DataTypes::Uint64:
			ret = std::to_string(*static_cast<const unsigned long long*>(data.Data));
			break;
		case DataTypes::Float:
			ret = std::to_string(*static_cast<const float*>(data.Data));
			break;
		case DataTypes::Double:
			ret = std::to_string(*static_cast<const double*>(data.Data));
			break;
		case DataTypes::Boolean:
			if (*static_cast<const bool*>(data.Data))
				ret = "true";
			else
				ret = "false";
			break;
		case DataTypes::String:
			ret = *static_cast<const std::string*>(data.Data);
			break;
		default:
			break;
		}

		return ret;
	}

}