#pragma once
#include "render/Material.h"
#include "render/RenderResource.h"
#include "GPUResourceOpenGL.h"

namespace engine
{
	struct GPUResourceMaterialOpenGLParams
	{
		inline GPUResourceMaterialOpenGLParams(std::span<const char> i_vetexShaderCode, std::span<const char> i_fragmentShaderCode)
			: vetexShaderCode(std::move(i_vetexShaderCode))
			, fragmentShaderCode(std::move(i_fragmentShaderCode))
		{

		}
		GPUResourceMaterialOpenGLParams() = default;
		GPUResourceMaterialOpenGLParams(const GPUResourceMaterialOpenGLParams&) = default;
		GPUResourceMaterialOpenGLParams(GPUResourceMaterialOpenGLParams&&) = default;

		std::span<const char> vetexShaderCode;
		std::span<const char> fragmentShaderCode;
	};

	class SpirVCompiler;
	class GPUResourceMaterialOpenGL : public GPUResourceOpenGL
	{
	public:
		GPUResourceMaterialOpenGL(const GPUResourceMaterialOpenGLParams& i_params, const SpirVCompiler& i_compiler);
		GPUResourceMaterialOpenGL(GPUResourceMaterialOpenGL&& i_other);
		~GPUResourceMaterialOpenGL();

		void Use();

	private:

		GLuint m_programId = std::numeric_limits<GLuint>::max();
	};
}