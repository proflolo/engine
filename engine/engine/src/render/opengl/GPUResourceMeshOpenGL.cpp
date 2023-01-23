#include "StdAfx.h"
#if ENGINE_OPENGL
#include "GPUResourceMeshOpenGL.h"
#include "OpenGLResult.h"

//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/

namespace engine
{
	GPUResourceMeshOpenGL::GPUResourceMeshOpenGL()
	{
		const GLfloat vertex_buffer_data[] = {
		   -1.0f, -1.0f, 0.0f,
		   1.0f, -1.0f, 0.0f,
		   0.0f,  1.0f, 0.0f,
		};

		std::vector<unsigned int> indices = {0, 1, 2};
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
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
			opengl_check();
		}
		glBindVertexArray(0);

		glGenBuffers(1, &m_indexBuffer);
		opengl_check();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
		opengl_check();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		opengl_check();
		m_indexSize = indices.size();
	}

	GPUResourceMeshOpenGL::~GPUResourceMeshOpenGL()
	{
		glDeleteBuffers(1, &m_vertexBuffer);
		opengl_check();
		glDeleteBuffers(1, &m_indexBuffer);
		opengl_check();
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
}
#endif