#include "StdAfx.h"
#include "GPUResourceMeshOpenGL.h"
#include "GPUResourceMaterialOpenGL.h"

namespace engine
{
	class OpenGLResult
	{
	public:
		OpenGLResult(GLuint i_error)
		{
			if (i_error != 0)
			{
				throw std::runtime_error("OpenGL Error");
			}
		}
	};
	GPUResourceMaterialOpenGL::GPUResourceMaterialOpenGL()
	{
		std::string vertexShader = "#version 330 core\
			layout(location = 0) in vec3 aPos; // the position variable has attribute position 0\
			out vec4 vertexColor; // specify a color output to the fragment shader\
			void main()\
			{\
				gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor\
				vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color\
			}";

		std::string fragmentShader = "#version 330 core\
			out vec4 FragColor;\
			in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  \
			void main()\
			{\
				FragColor = vertexColor;\
			}";

		glCreateShader(GL_VERTEX_SHADER);


	}

}