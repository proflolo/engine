#include "StdAfx.h"
#include "GameModule.h"

namespace engine
{
	int GameModule::Run()
	{
		std::vector<std::unique_ptr<Module>> m_subModules = InitRequiredModules();
		return 0;
	}

	const std::vector<std::unique_ptr<Module>>& GameModule::GetSubModules() const
	{
		return m_subModules;
	}
}



