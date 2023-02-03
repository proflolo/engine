#pragma once
#if ENGINE_OPENGL
#include "GPUResourceOpenGL.h"

namespace engine
{
	class SpirVCompiler;
	class GPUResourceMaterialOpenGL : public GPUResourceOpenGL
	{
	public:
		GPUResourceMaterialOpenGL(const SpirVCompiler& i_compiler);
		~GPUResourceMaterialOpenGL();

		void Use();

	private:
		
		GLuint m_programId;
	};
}
#endif