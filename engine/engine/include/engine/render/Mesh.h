#pragma once

namespace engine
{
	class GPUResource;

	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		bool HasGPUResource() const;
		void AssignGPUResource(std::shared_ptr<GPUResource> i_resource);
	private:
		std::shared_ptr<GPUResource> m_resource;
	};
}