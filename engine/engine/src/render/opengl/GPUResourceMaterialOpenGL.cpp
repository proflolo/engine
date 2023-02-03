#include "StdAfx.h"
#if ENGINE_OPENGL
#include "GPUResourceMeshOpenGL.h"
#include "GPUResourceMaterialOpenGL.h"
#include "OpenGLResult.h"
#include "render/shadercompiler/SpirVCompiler.h"
#include <fstream>      // std::ifstream

namespace engine
{
	GPUResourceMaterialOpenGL::GPUResourceMaterialOpenGL(const SpirVCompiler& i_compiler)
	{
		std::string vertexShaderCode = "#version 450\n\
			layout(location = 0) in vec3 aPos; // the position variable has attribute position 0\n\
			layout(location = 0) out vec4 vertexColor; // specify a color output to the fragment shader\n\
			void main()\n\
			{\n\
				gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor\n\
				vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color\n\
			}";

		std::string fragmentShaderCode = "#version 430 core\n\
			layout(location = 0) out vec4 FragColor;\n\
			layout(location = 0) in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  \n\
			void main()\n\
			{\n\
				FragColor = vertexColor;\n\
			}";

		//i_compiler.GLSLtoSPV(SpirVCompiler::ShaderStage::Fragment, fragmentShaderCode);
		//i_compiler.SPVtoGLSL(i_compiler.GLSLtoSPV(SpirVCompiler::ShaderStage::Vertex, vertexShaderCode));
		//
		std::vector<unsigned int> vertexShaderCompiled = i_compiler.GLSLtoSPV(SpirVCompiler::ShaderStage::Vertex, vertexShaderCode);
		std::vector<unsigned int> fragmentShaderCompiled = i_compiler.GLSLtoSPV(SpirVCompiler::ShaderStage::Fragment, fragmentShaderCode);
		
		

		//std::ifstream file("frag.spv", std::ifstream::in);
		//std::vector<unsigned char> fragmentShader(std::istreambuf_iterator<char>(file), {});

		GLuint vertexId;
		GLuint fragmentId;

		vertexId = glCreateShader(GL_VERTEX_SHADER);
		opengl_check();

		fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
		opengl_check();

#if SPIRV_OPENGL
		glShaderBinary(1, &fragmentId, GL_SHADER_BINARY_FORMAT_SPIR_V, fragmentShaderCompiled.data(), fragmentShaderCompiled.size());
		opengl_check();
		
		glShaderBinary(1, &vertexId, GL_SHADER_BINARY_FORMAT_SPIR_V, vertexShaderCompiled.data(), vertexShaderCompiled.size());
		opengl_check();

		std::string fsEntrypoint = "main"; // Get VS entry point name
		glSpecializeShader(fragmentId, (const GLchar*)fsEntrypoint.c_str(), 0, nullptr, nullptr);
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

		std::string vsEntrypoint = "main"; // Get VS entry point name
		glSpecializeShader(vertexId, (const GLchar*)vsEntrypoint.c_str(), 0, nullptr, nullptr);
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

#else
		std::string wtvert = i_compiler.SPVtoGLSL(vertexShaderCompiled);
		std::string wtfrag = i_compiler.SPVtoGLSL(fragmentShaderCompiled);

		
		const char* fragmentCharPtr = wtfrag.c_str();
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

		const char* vertexCharPtr = wtvert.c_str();
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
#endif
		
		

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