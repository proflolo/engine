#include "StdAfx.h"
#include "GameModule.h"

namespace engine
{
	int GameModule::Run()
	{
		std::vector<std::unique_ptr<Module>> modules = InitRequiredModules();
		return 0;
	}
}

extern std::unique_ptr<engine::GameModule> CreateGame();

#if !WITH_EDITOR
int main(int argc, char** argv)
{
	std::unique_ptr<engine::GameModule> game = CreateGame();
	return game->Run();
}
#endif