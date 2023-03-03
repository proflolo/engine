#include "StdAfx.h"
#include "Game.h"
#include "engine/render/Renderer.h"
#include "engine/render/Material.h"
#include "engine/render/Mesh.h"
#include "engine/db/ShaderAsset.h"

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

	template<typename Func>
	static void visit_assets(const Func& i_func)
	{

	}
};

engine::StaticMesh<Game::VertexData> Game::SampleMesh()
{
	std::vector<VertexData> dtaa;
	dtaa.emplace_back(-1.0f, -1.0f, 0.0f);
	dtaa.emplace_back(0.0f, -1.0f, 0.0f);
	dtaa.emplace_back(-1.0f, 1.0f, 0.0f);
	dtaa.emplace_back(1.0f, -1.0f, 0.0f);
	dtaa.emplace_back(1.0f, 1.0f, 0.0f);
	
	std::vector<unsigned int> indices = { 0, 1, 2, 1, 3, 4 };

	return engine::StaticMesh<VertexData>(std::move(dtaa), std::move(indices));
}

std::shared_ptr<engine::Asset<engine::ShaderAssetVertex>> SampleVertexShader()
{
	return std::make_shared<engine::Asset<engine::ShaderAssetVertex>>("shaders/shader1.vs", engine::ShaderAssetVertex());
}

std::shared_ptr<engine::Asset<engine::ShaderAssetFragment>> SampleFragmentShader()
{
	return std::make_shared<engine::Asset<engine::ShaderAssetFragment>>("shaders/shader1.fs", engine::ShaderAssetFragment());
}

Game::Game(const engine::Context& i_context)
	: engine::GameModule(i_context)
	, m_gameDb(std::make_unique<Gamedb>())
	, m_mesh(SampleMesh())
	, m_material(SampleVertexShader(), SampleFragmentShader())
{

}

Game::~Game()
{
}

const engine::Db& Game::GetDefinitions() const
{
	return *m_gameDb;
}

std::vector<std::unique_ptr<engine::Module>> Game::CreateDependencies() const
{
	std::vector<std::unique_ptr<engine::Module>> modules;
	modules.emplace_back(std::make_unique<render2D::Render2DModule>(GetEngineContext()));
	return modules;
}

engine::RenderClient& Game::GetRenderClient()
{
	return *this;
}

engine::UpdateClient& Game::GetUpdateClient()
{
	return *this;
}

void Game::Update()
{

}

void Game::Render(std::stop_token i_stopToken, engine::Renderer& i_renderer)
{
	i_renderer.Render(m_mesh, m_material);
}

namespace engine
{
	std::unique_ptr<engine::GameModule> CreateGame(const engine::Context& i_context)
	{
		return std::make_unique<Game>(i_context);
	}
}

