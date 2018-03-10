#pragma once

#ifdef _GLEW
#pragma comment(lib, "glew32")
#include <GLEW/glew.h>
#include <GLEW/wglew.h>
#else
#include <Graphics/GL4/glextension/GLExtension.h>

#endif
