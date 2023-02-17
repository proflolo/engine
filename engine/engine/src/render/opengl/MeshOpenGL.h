 #pragma once
#if ENGINE_OPENGL
#include "GPUResourceOpenGL.h"
#include "render/RenderResource.h"
#include "render/Mesh.h"
namespace engine
{
	class GPUResourceMeshOpenGL : public GPUResourceOpenGL
	{
	public:
		GPUResourceMeshOpenGL(const MeshGeneric& i_mesh);
		GPUResourceMeshOpenGL(GPUResourceMeshOpenGL&& i_other);
		~GPUResourceMeshOpenGL();

		struct BindHandle {};

		GLuint GetVertexArray() const;
		GLuint GetVertexBuffer() const;
		GLuint GetIndexBuffer() const;
		GLsizei GetIndexSize() const;

	private:
		GLuint m_vertexArray = std::numeric_limits<GLuint>::max();
		GLuint m_vertexBuffer = std::numeric_limits<GLuint>::max();
		GLuint m_indexBuffer = std::numeric_limits<GLuint>::max();
		GLsizei m_indexSize = std::numeric_limits<GLsizei>::max();
	};
}
#endif