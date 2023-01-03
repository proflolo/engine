#pragma once

namespace engine
{
	class Mesh;
	class Material;

	class Renderer
	{
	public:
		virtual void Render(Mesh& i_mesh, Material& i_material) = 0;
	};
}