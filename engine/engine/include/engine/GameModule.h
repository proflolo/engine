#pragma once
#include "Module.h"

namespace engine
{
	class GameModule : public Module
	{
	public:
		int Run();

		void Init();
		const std::vector<std::unique_ptr<Module>>& GetSubModules() const;
	protected:
		virtual std::vector<std::unique_ptr<Module>> InitRequiredModules() const = 0;

	private:
		std::vector<std::unique_ptr<Module>> m_subModules;
	};

	extern std::unique_ptr<engine::GameModule> CreateGame();

}