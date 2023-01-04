#pragma once
#include "engine/GameModule.h"
#include "engine/render/RenderClient.h"

#include "engine/render/Material.h"
#include "engine/render/Mesh.h"

class Gamedb;

class Game : public engine::GameModule, public engine::RenderClient
{
public:
	Game();

	~Game();


	const engine::definitions& GetDefinitions() const override;
	std::vector<std::unique_ptr<engine::Module>> InitRequiredModules() const override;

	virtual RenderClient& GetRenderClient() override;
	void Render(std::stop_token i_stopToken, engine::Renderer& i_renderer) override;

private:
	std::unique_ptr<Gamedb> m_gameDb;

	engine::Material m_material;
	engine::Mesh m_mesh;
};

