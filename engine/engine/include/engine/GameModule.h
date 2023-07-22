#pragma once
#include "Module.h"

namespace engine
{
	
	class GameModule : public ModuleImplementation<GameModule>
	{
	public:
		virtual std::vector<std::unique_ptr<Module>> CreateDependencies() const = 0;

	protected:
		GameModule(const Context& i_context);

	private:
	};

	extern std::unique_ptr<engine::GameModule> CreateGame(const engine::Context& i_context);

}