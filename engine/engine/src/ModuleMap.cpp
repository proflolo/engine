#include "StdAfx.h"
#include "ModuleMap.h"
#include "Module.h"
namespace engine
{
	ModuleMap::ModuleMap(std::vector<std::unique_ptr<Module>> i_modules)
	{
		for (size_t i = 0; i < i_modules.size(); ++i)
		{
			std::unique_ptr<Module> modul = std::move(i_modules[i]);
			m_modules.emplace(modul->ModuleId(), std::move(modul));
		}
	}
}