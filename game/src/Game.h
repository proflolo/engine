#pragma once
#include "engine/GameModule.h"
#include "engine/render/RenderClient.h"
#include "engine/update/UpdateClient.h"

class Gamedb;



class Game : public engine::GameModule, public engine::RenderClient, public engine::UpdateClient
{
public:
	Game(const engine::Context& i_context);

	~Game();


	const engine::Db& GetDefinitions() const override;
	std::vector<std::unique_ptr<engine::Module>> CreateDependencies() const override;

	engine::RenderClient& GetRenderClient() override;
	engine::UpdateClient& GetUpdateClient() override;

	void Update() override;
	void Render(std::stop_token i_stopToken, engine::Renderer& i_renderer) override;

private:
	std::unique_ptr<Gamedb> m_gameDb;

	

	struct VertexData
	{
		VertexData() = default;
		VertexData(float x, float y, float z)
			:vertex(x,y,z)
		{

		}
		engine::math::vector<float, 3> vertex;
	};

	engine::StaticMesh<VertexData> SampleMesh();

	engine::Material<VertexData> m_material;

	engine::StaticMesh<VertexData> m_mesh;
};

