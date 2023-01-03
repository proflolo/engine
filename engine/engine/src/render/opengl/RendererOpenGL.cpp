#include "StdAfx.h"
#include "RendererOpenGL.h"
#include "render/Mesh.h"
#include "GPUResourceMeshOpenGL.h"
namespace engine
{
	class GPUResourceHolder: public GPUResource
	{
	public:
		GPUResourceHolder(RendererOpenGL& i_renderer, std::unique_ptr<GPUResourceOpenGL> i_realResource)
			: renderer(&i_renderer)
			, realResource(std::move(i_realResource))
		{

		}

		~GPUResourceHolder()
		{
			if (renderer)
			{
				renderer->FlagResourceForDeletion(Extract());
			}
		}

		std::unique_ptr<GPUResourceOpenGL> Extract()
		{
			renderer = nullptr;
			return std::move(realResource);
		}

		RendererOpenGL* renderer;
		std::unique_ptr<GPUResourceOpenGL> realResource;
	};

	RendererOpenGL::RendererOpenGL()
	{
	}

	RendererOpenGL::~RendererOpenGL()
	{
		for (const std::weak_ptr<GPUResourceHolder>& activeResource : m_activeResources)
		{
			if (std::shared_ptr<GPUResourceHolder> resource = activeResource.lock())
			{
				resource->Extract();
			}
		}
	}

	void RendererOpenGL::BeginRender(std::stop_token i_stopToken)
	{
		glClearColor(0.129f, 0.586f, 0.949f, 1.0f);	// rgb(33,150,243)
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RendererOpenGL::EndRender(std::stop_token i_stopToken)
	{
		{
			std::scoped_lock lock(m_deleteMutex);
			decltype(m_toDeleteResources) toDelete;
			std::swap(toDelete, m_toDeleteResources);
		}
	}

	void RendererOpenGL::Render(Mesh& i_mesh)
	{
		if (!i_mesh.HasGPUResource())
		{
			//Create it!
			std::unique_ptr<GPUResourceMeshOpenGL> gpuResource = std::make_unique<GPUResourceMeshOpenGL>();
			std::shared_ptr<GPUResourceHolder> handle = std::make_shared<GPUResourceHolder>(*this, std::move(gpuResource));
			m_activeResources.emplace_back(handle);
			i_mesh.AssignGPUResource(std::move(handle));
		}
	}

	void RendererOpenGL::FlagResourceForDeletion(std::unique_ptr<GPUResourceOpenGL> i_resource)
	{
		std::scoped_lock lock(m_deleteMutex);
		m_toDeleteResources.emplace_back(std::move(i_resource));
	}
}