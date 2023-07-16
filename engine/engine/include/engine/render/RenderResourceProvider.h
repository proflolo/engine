#pragma once

namespace engine
{
	class MaterialGeneric;
	class MeshGeneric;

	class RenderResourceProvider
	{
	public:
		virtual std::shared_future<void> Load(const RenderResource<void>& i_resource) = 0;
		virtual std::shared_future<void> Load(const MaterialGeneric& i_material) = 0;
		virtual std::shared_future<void> Load(const MeshGeneric& i_material) = 0;

	protected:
		RenderResourceProvider() = default;
	};
}