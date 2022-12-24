#pragma once
#include "engine/GameModule.h"

class Gamedb;

class Game : public engine::GameModule
{
public:
	Game();

	~Game();

	const engine::definitions& GetDefinitions() const override;
	std::vector<std::unique_ptr<engine::Module>> InitRequiredModules() const override;

	std::unique_ptr<Gamedb> m_gameDb;
};

