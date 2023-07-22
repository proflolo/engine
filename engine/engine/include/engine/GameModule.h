#pragma once
#include "Module.h"


namespace engine
{
	class ModuleMap;
	class GameModule : public ModuleImplementation<GameModule>
	{
	public:

	protected:
		GameModule(const Context& i_context, const engine::ModuleMap& i_modules);

	protected:

		const engine::ModuleMap& GetModules() const
		{
			return m_modules;
		}

	private:
		const engine::ModuleMap& m_modules;
	};

	//Called first
	extern std::vector<std::unique_ptr<Module>> CreateModules(const engine::Context& i_context);
	//Called next
	extern std::unique_ptr<engine::GameModule> CreateGame(const engine::Context& i_context, const engine::ModuleMap& i_modules);

}