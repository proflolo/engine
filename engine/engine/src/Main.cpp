#include "StdAfx.h"
#include <Windows.h>
#include "window/WindowWindowsOpenGL.h"
#include "window/WindowWindowsVulkan.h"
#include "GameModule.h"
#include "render/RenderClientCombiner.h"
#include "update/UpdateClientCombiner.h"
#include "EngineFactory.h"
#include "render/opengl/RenderResourceProviderOpenGL.h"
#include "ModuleMap.h"

#if !WITH_EDITOR

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	engine::Context context = engine::EngineFactory::CreateContext();
	std::unique_ptr<engine::GameModule> gameModule = engine::CreateGame(context);
	std::vector<std::unique_ptr<engine::Module>> depndentModules = gameModule->CreateDependencies();
	std::vector<std::reference_wrapper<engine::RenderClient>> renderClients = {gameModule->GetRenderClient()};
	std::vector<std::reference_wrapper<engine::UpdateClient>> updateClients = { gameModule->GetUpdateClient() };
	for (const std::unique_ptr<engine::Module>& subModule : depndentModules)
	{
		renderClients.emplace_back(subModule.get()->GetRenderClient());
		updateClients.emplace_back(subModule.get()->GetUpdateClient());
	}
	engine::ModuleMap modules(std::move(depndentModules));

	engine::RenderClientCombiner renderCombiner(std::move(renderClients));
	engine::UpdateClientCombiner updateCombiner(std::move(updateClients));

#if ENGINE_OPENGL
	//1. Create Window
	engine::WindowWindowsOpenGL window(hInstance, nCmdShow, renderCombiner, updateCombiner);
	return window.Run();
#else ENGINE_VULKAN
	engine::WindowWindowsVulkan window(hInstance, nCmdShow, gameModule->GetRenderClient());
	return window.Run();
#endif
}
#endif