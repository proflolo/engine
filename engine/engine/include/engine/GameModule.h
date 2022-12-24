#pragma once
#include "Module.h"

namespace engine
{
	class GameModule : public Module
	{
	public:
		int Run();

	protected:
		virtual std::vector<std::unique_ptr<Module>> InitRequiredModules() const = 0;
	};
}