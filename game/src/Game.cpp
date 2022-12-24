#include "StdAfx.h"
#include "Game.h"


class Character: public engine::definition<Character, "Char", editor::ui_name("C")>
{
	engine::field<int, "level"> level2;

public:
	template<typename Func>
	static void visit_fields(const Func& i_func)
	{
		visit_field(&Character::level2, i_func);
	}
};

class Gamedb : public engine::definitionbase<Gamedb>
{
public:
	template<typename Func>
	static void visit_types(const Func& i_func)
	{
		visit_type<Character>(i_func);
	}
};


Game::Game()
	: m_gameDb(std::make_unique<Gamedb>())
{
}

Game::~Game()
{
}

const engine::definitions& Game::GetDefinitions() const
{
	return *m_gameDb;
}

std::vector<std::unique_ptr<engine::Module>>Game::InitRequiredModules() const
{
	std::vector<std::unique_ptr<engine::Module>> modules;
	modules.emplace_back(std::make_unique<render2D::Render2DModule>());
	return modules;
}

namespace engine
{
	std::unique_ptr<engine::GameModule> CreateGame()
	{
		return std::make_unique<Game>();
	}
}

