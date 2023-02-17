#pragma once
#include "render/Material.h"
#include "render/RenderResource.h"
#include "GPUResourceOpenGL.h"

namespace engine
{
	class SpirVCompiler;
	class GPUResourceMaterialOpenGL : public GPUResourceOpenGL
	{
	public:
		GPUResourceMaterialOpenGL(const SpirVCompiler& i_compiler);
		GPUResourceMaterialOpenGL(GPUResourceMaterialOpenGL&& i_other);
		~GPUResourceMaterialOpenGL();

		void Use();

	private:

		GLuint m_programId = std::numeric_limits<GLuint>::max();
	};
}