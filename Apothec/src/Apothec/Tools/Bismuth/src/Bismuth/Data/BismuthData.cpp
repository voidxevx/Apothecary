#include "BismuthData.h"

namespace bismuth
{
	DataRegistry* DataRegistry::s_Instance = nullptr;

	IDataType::IDataType(const std::string& alias)
	{
		DataRegistry::Get()->RegisterDataType(alias, this);
	}

}