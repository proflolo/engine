#pragma once

namespace engine
{
	class GPUResource;

	class Resource
	{
	public:
		Resource();
		~Resource();

		bool HasGPUResource() const;
		void AssignGPUResource(std::shared_ptr<GPUResource> i_resource);
		template<typename GPUResourceT>
		inline GPUResourceT& GetGPUResource()
		{
			return static_cast<GPUResourceT&>(*m_resource);
		}

	private:
		std::shared_ptr<GPUResource> m_resource;
	};
}