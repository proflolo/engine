#pragma once
#if ENGINE_OPENGL
#include "GPUResourceOpenGL.h"

namespace engine
{
	class GPUResourceMeshOpenGL : public GPUResourceOpenGL
	{
	public:
		GPUResourceMeshOpenGL();
		~GPUResourceMeshOpenGL();

		struct BindHandle {};

		GLuint GetVertexArray() const;
		GLuint GetVertexBuffer() const;
		GLuint GetIndexBuffer() const;
		GLsizei GetIndexSize() const;

	private:
		GLuint m_vertexArray;
		GLuint m_vertexBuffer;
		GLuint m_indexBuffer;
		GLsizei m_indexSize;
	};
}
#endif