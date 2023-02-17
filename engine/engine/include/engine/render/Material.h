#pragma once
#include "Resource.h"
#include "RenderResource.h"

namespace engine
{
	class MaterialGeneric: public Resource, public RenderResource<MaterialGeneric>
	{
	public:
		~MaterialGeneric();

	protected:
		MaterialGeneric();
	};

	template<typename Layout>
	class Material : public MaterialGeneric
	{
	public:
		Material() = default;
	};
}