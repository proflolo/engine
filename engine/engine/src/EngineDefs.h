#pragma once


//Editor builds MUST be in OpenGL
#if WITH_EDITOR
#	define ENGINE_OPENGL 1
#	define PLATFORM_JUCE 1
#	define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED
#elif _WIN32
#	define ENGINE_OPENGL 1
#	define PLATFORM_WINDOWS 1
#endif





#if PLATFORM_WINDOWS
#include <Windows.h>
#endif

#if ENGINE_OPENGL
#if WITH_EDITOR
//#include <gl/GL.h>
#include "juce_opengl/opengl/juce_gl.h"
using namespace juce::gl;
#else
#include <gl/GL.h>
#include "platform/opengl/wglext.h"
#include "platform/opengl/glext.h"
#include "platform/opengl/glcorearb.h"
#endif
#endif