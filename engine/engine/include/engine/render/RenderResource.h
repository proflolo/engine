#pragma once

namespace engine
{	
	template<typename T>
	struct RenderResource
	{
		using Id = size_t;
		bool HasRenderResource() const
		{
			return renderResource != std::numeric_limits<size_t>::max();
		}

		mutable size_t renderResource = std::numeric_limits<size_t>::max();
	};
}