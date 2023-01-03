#pragma once

namespace engine
{
	class Mesh;

	class Renderer
	{
	public:
		virtual void Render(Mesh& i_mesh) = 0;
	};
}