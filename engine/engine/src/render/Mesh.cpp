#include "StdAfx.h"
#include "engine/render/Mesh.h"

namespace engine
{
	Mesh::Mesh()
	{

	}

	Mesh::~Mesh()
	{

	}

	bool Mesh::HasGPUResource() const
	{
		return m_resource != nullptr;
	}

	void Mesh::AssignGPUResource(std::shared_ptr<GPUResource> i_resource)
	{
		m_resource = std::move(i_resource);
	}

}