#include "StdAfx.h"
#include "render/Resource.h"

namespace engine
{

	Resource::Resource()
	{

	}

	Resource::~Resource()
	{

	}

	bool Resource::HasGPUResource() const
	{
		return m_resource != nullptr;
	}

	void Resource::AssignGPUResource(std::shared_ptr<GPUResource> i_resource)
	{
		m_resource = std::move(i_resource);
	}
}