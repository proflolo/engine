#pragma once
#if ENGINE_OPENGL
#include "GPUResourceOpenGL.h"

namespace engine
{
	class GPUResourceMaterialOpenGL : public GPUResourceOpenGL
	{
	public:
		GPUResourceMaterialOpenGL();
		~GPUResourceMaterialOpenGL();

		void Use();

	private:
		
		GLuint m_programId;
	};
}
#endif