#include "StdAfx.h"
#include "GameModule.h"

namespace engine
{
	GameModule::GameModule(const Context& i_context, const engine::ModuleMap& i_modules)
		: ModuleImplementation<GameModule>(i_context)
		, m_modules(i_modules)
	{

	}
}




