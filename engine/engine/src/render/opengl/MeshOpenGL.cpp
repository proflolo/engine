#include "StdAfx.h"
#if ENGINE_OPENGL
#include "MeshOpenGL.h"
#include "OpenGLResult.h"

//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/

namespace engine
{
	GPUResourceMeshOpenGL::GPUResourceMeshOpenGL(const GPUResourceMeshOpenGLParams& i_params)
	{
		//const GLfloat vertex_buffer_data[] = {
		//   -1.0f, -1.0f, 0.0f,
		//   1.0f, -1.0f, 0.0f,
		//   0.0f,  1.0f, 0.0f,
		//};
		//
		//std::vector<unsigned int> indices = {0, 1, 2};
		
		
		glGenVertexArrays(1, &m_vertexArray);
		glBindVertexArray(m_vertexArray);
		{
			// Generate 1 buffer, put the resulting identifier in vertexbuffer
			glGenBuffers(1, &m_vertexBuffer);
			opengl_check();
			// The following commands will talk about our 'vertexbuffer' buffer
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
			opengl_check();
			// Give our vertices to OpenGL.
			const auto& data = i_params.data;
			//auto s = sizeof(vertex_buffer_data);
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
			opengl_check();
		}
		glBindVertexArray(0);

		glGenBuffers(1, &m_indexBuffer);
		opengl_check();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
		opengl_check();

		const auto& indices = i_params.indices;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		opengl_check();
		m_indexSize = indices.size();
	}

	

	GPUResourceMeshOpenGL::GPUResourceMeshOpenGL(GPUResourceMeshOpenGL&& i_other)
		: m_vertexArray(std::move(i_other.m_vertexArray))
		, m_vertexBuffer(std::move(i_other.m_vertexBuffer))
		, m_indexBuffer(std::move(i_other.m_indexBuffer))
		, m_indexSize(std::move(i_other.m_indexSize))
	{
		i_other.m_vertexArray = std::numeric_limits<GLuint>::max();
		i_other.m_vertexBuffer = std::numeric_limits<GLuint>::max();
		i_other.m_indexBuffer = std::numeric_limits<GLuint>::max();
		i_other.m_indexSize = std::numeric_limits<GLuint>::max();
	}

	GPUResourceMeshOpenGL::~GPUResourceMeshOpenGL()
	{
		if (m_vertexBuffer != std::numeric_limits<GLuint>::max())
		{
			glDeleteBuffers(1, &m_vertexBuffer);
			opengl_check();
		}
		if (m_indexBuffer != std::numeric_limits<GLuint>::max())
		{
			glDeleteBuffers(1, &m_indexBuffer);
			opengl_check();
		}
	}


	
	
	GLuint GPUResourceMeshOpenGL::GetVertexArray() const
	{
		return m_vertexArray;
	}
	GLuint GPUResourceMeshOpenGL::GetVertexBuffer() const
	{
		return m_vertexBuffer;
	}
	
	GLuint GPUResourceMeshOpenGL::GetIndexBuffer() const
	{
		return m_indexBuffer;
	}
	
	GLsizei GPUResourceMeshOpenGL::GetIndexSize() const
	{
		return m_indexSize;
	}

	GPUResourceMeshOpenGLParams::GPUResourceMeshOpenGLParams(std::vector<float> i_data, std::vector<unsigned int> i_indices)
		: data(std::move(i_data))
		, indices(std::move(i_indices))
	{
	}
}
#endif