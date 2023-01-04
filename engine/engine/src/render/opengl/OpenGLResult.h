#pragma once

namespace engine
{
	inline void opengl_check()
	{
		GLenum err = glGetError();
		if (err != 0)
		{
			throw std::runtime_error("OpenGL Error");
		}
	}

}