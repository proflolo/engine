#include "StdAfx.h"
#include <Windows.h>
#include "window/WindowWindowsOpenGL.h"
#include "GameModule.h"

#if !WITH_EDITOR

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	std::unique_ptr<engine::GameModule> gameModule = engine::CreateGame();
	//1. Create Window
	engine::WindowWindowsOpenGL window(hInstance, nCmdShow, gameModule->GetRenderClient());
	return window.Run();
}
#endif