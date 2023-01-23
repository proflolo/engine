#include "StdAfx.h"
#if ENGINE_OPENGL
#include "GPUResourceMeshOpenGL.h"
#include "GPUResourceMaterialOpenGL.h"
#include "OpenGLResult.h"

namespace engine
{
	GPUResourceMaterialOpenGL::GPUResourceMaterialOpenGL()
	{
		std::string vertexShader = "#version 430 core\n\
			layout(location = 0) in vec3 aPos; // the position variable has attribute position 0\n\
			out vec4 vertexColor; // specify a color output to the fragment shader\n\
			void main()\n\
			{\n\
				gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor\n\
				vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color\n\
			}";

		std::string fragmentShader = "#version 430 core\n\
			out vec4 FragColor;\n\
			in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  \n\
			void main()\n\
			{\n\
				FragColor = vertexColor;\n\
			}";

		GLuint vertexId;
		GLuint fragmentId;

		vertexId = glCreateShader(GL_VERTEX_SHADER);
		opengl_check();

		fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
		opengl_check();

		const char* vertexCharPtr = vertexShader.c_str();
		glShaderSource(vertexId, 1, &vertexCharPtr, NULL);
		opengl_check();

		glCompileShader(vertexId);
		{
			int success = 0;
			glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
			if (success == 0)
			{
				char infoLog[512];
				glGetShaderInfoLog(vertexId, 512, NULL, infoLog);
				throw std::runtime_error(infoLog);
			}
		}
		opengl_check();

		const char* fragmentCharPtr = fragmentShader.c_str();
		glShaderSource(fragmentId, 1, &fragmentCharPtr, NULL);
		opengl_check();

		glCompileShader(fragmentId);
		{
			int success = 0;
			glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
			if (success == 0)
			{
				char infoLog[512];
				glGetShaderInfoLog(fragmentId, 512, NULL, infoLog);
				throw std::runtime_error(infoLog);
			}
		}
		opengl_check();

		m_programId = glCreateProgram();
		opengl_check();
		glAttachShader(m_programId, vertexId);
		opengl_check();
		glAttachShader(m_programId, fragmentId);
		opengl_check();
		glLinkProgram(m_programId);

		{
			int success = 0;
			glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
			if (success == 0)
			{
				char infoLog[512];
				glGetProgramInfoLog(m_programId, 512, NULL, infoLog);
				throw std::runtime_error(infoLog);
			}
		}

		glDeleteShader(vertexId);
		opengl_check();
		glDeleteShader(fragmentId);
		opengl_check();
	}

	GPUResourceMaterialOpenGL::~GPUResourceMaterialOpenGL()
	{
		glDeleteProgram(m_programId);
		opengl_check();
	}

	void GPUResourceMaterialOpenGL::Use()
	{
		glUseProgram(m_programId);
		opengl_check();
		
	}

}
#endif