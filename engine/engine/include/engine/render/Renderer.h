#pragma once

namespace engine
{
	class MeshGeneric;
	class MaterialGeneric;
	struct RenderLayout;

	class Renderer
	{
	public:
		virtual void Render(MeshGeneric& i_mesh, MaterialGeneric& i_material, const RenderLayout& i_layout) = 0;
		template<typename Layout>
		inline void Render(Mesh<Layout>& i_mesh, Material<Layout>& i_material)
		{
			Render(i_mesh, i_material, i_mesh.GetLayout());
		}
	};
}