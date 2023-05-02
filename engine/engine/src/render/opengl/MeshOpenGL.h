 #pragma once
#if ENGINE_OPENGL
#include "GPUResourceOpenGL.h"
#include "render/RenderResource.h"
#include "render/Mesh.h"
namespace engine
{
	struct GPUResourceMeshOpenGLParams
	{
		GPUResourceMeshOpenGLParams() = default;
		GPUResourceMeshOpenGLParams(std::vector<float> i_data, std::vector<unsigned int> i_indices);
		std::vector<float> data;
		std::vector<unsigned int> indices;
	};

	class GPUResourceMeshOpenGL : public GPUResourceOpenGL
	{
	public:
		GPUResourceMeshOpenGL(const GPUResourceMeshOpenGLParams& i_params);
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