#include "StdAfx.h"
#include "Module.h"

namespace engine
{

	const Context& engine::Module::GetEngineContext() const
	{
		return m_context;
	}

	Module::Module(const Context& i_context)
		: m_context(i_context)
	{

	}

}