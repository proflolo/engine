#pragma once

namespace engine
{	
	template<typename T>
	struct RenderResource: public RenderResource<void>
	{
		bool HasRenderResource() const
		{
			return renderResource != std::numeric_limits<size_t>::max();
		}

		mutable size_t renderResource = std::numeric_limits<size_t>::max();

	protected:
		RenderResource()
			: RenderResource<void>(T::k_renderType)
		{

		}
	};

	template<>
	struct RenderResource<void>
	{
		size_t renderType;

	protected:
		RenderResource<void>(size_t i_renderType)
			: renderType(i_renderType)
		{

		}
	};
}