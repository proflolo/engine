#include "StdAfx.h"
#include "ModuleMap.h"
#include "Module.h"
namespace engine
{
	ModuleMap::ModuleMap(std::vector<std::unique_ptr<Module>> i_modules)
		: m_modules(std::move(i_modules))
	{
		
	}
}