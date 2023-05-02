#pragma once

namespace engine
{
	class MaterialGeneric;
	class MeshGeneric;

	class RenderResourceProvider
	{
	public:
		virtual std::future<RenderResource<MaterialGeneric>::Id> Load(const MaterialGeneric& i_material) = 0;
		virtual std::future<RenderResource<MeshGeneric>::Id> Load(const MeshGeneric& i_material) = 0;

	protected:
		RenderResourceProvider() = default;
	};
}