#pragma once

#pragma comment(lib, "Opengl32")

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

//TODO : Optimize extensions
#include <GL/wglext.h>		//(up to 4.6)
#include <GL/glext.h>		//(up to 3.3)
#include <GL/GL.h>
#include <assert.h>

namespace WGL
{
	bool InitWGL();
	bool InitGLEXT();
}

#define GL_DECL_PFN(type, func) \
do{ \
	func = reinterpret_cast<type>(wglGetProcAddress(#func));\
	if(!func){ \
		assert(0 && "failed to load func pointer"); \
	}\
}while(false)	// run once


//====================== WGL ============================
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

//======================= GL EXT ======================== (Add Func what u want)
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;

extern PFNGLENABLEVERTEXATTRIBARRAYPROC	glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC		glVertexAttribPointer;
extern PFNGLBINDVERTEXARRAYPROC			glBindVertexArray;
extern PFNGLGENVERTEXARRAYSPROC			glGenVertexArrays;
extern PFNGLCREATEVERTEXARRAYSPROC		glCreateVertexArrays;
extern PFNGLVERTEXATTRIBDIVISORPROC		glVertexAttribDivisor;
extern PFNGLDRAWARRAYSINSTANCEDPROC		glDrawArraysInstanced;

extern PFNGLDELETEBUFFERSPROC			glDeleteBuffers;
extern PFNGLDELETEVERTEXARRAYSPROC		glDeleteVertexArrays;

//===================== PROGRAM ==========================
extern PFNGLCREATEPROGRAMPROC			glCreateProgram;
extern PFNGLATTACHSHADERPROC			glAttachShader;
extern PFNGLLINKPROGRAMPROC				glLinkProgram;
extern PFNGLGETPROGRAMIVPROC			glGetProgramiv;
extern PFNGLDELETESHADERPROC			glDeleteShader;
extern PFNGLDELETEPROGRAMPROC			glDeleteProgram;
extern PFNGLUSEPROGRAMPROC				glUseProgram;
extern PFNGLCREATESHADERPROC			glCreateShader;
extern PFNGLSHADERSOURCEPROC			glShaderSource;
extern PFNGLCOMPILESHADERPROC			glCompileShader;
extern PFNGLGETSHADERIVPROC				glGetShaderiv;
extern PFNGLGETPROGRAMINFOLOGPROC		glGetProgramInfoLog;
extern PFNGLGETSHADERINFOLOGPROC		glGetShaderInfoLog;

//=================== TEXTURE ===========================

extern PFNGLACTIVETEXTUREPROC			glActiveTexture;
extern PFNGLCREATETEXTURESPROC			glCreateTextures;
extern PFNGLTEXTUREPARAMETERIPROC		glTextureParameteri;
extern PFNGLBINDTEXTUREUNITPROC			glBindTextureUnit;

//=================== EXT ===============================
extern PFNGLNAMEDBUFFERSUBDATAEXTPROC		glNamedBufferSubDataEXT;
extern PFNGLBINDBUFFERBASEPROC				glBindBufferBase;
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC		glCompressedTexImage2DARB;


//================== GL 4.4 ==============================

extern PFNGLCLIPCONTROLPROC				glClipControl;
