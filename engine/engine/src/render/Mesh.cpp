#include "StdAfx.h"
#include "engine/render/Mesh.h"

namespace engine
{
	MeshGeneric::~MeshGeneric()
	{

	}

	MeshGeneric::MeshGeneric(std::vector<unsigned int> i_indices)
		: m_indices(std::move(i_indices))
	{

	}

	

}