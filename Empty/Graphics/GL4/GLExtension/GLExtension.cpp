#include <Core/PCH.h>
#include <graphics/GL4/glextension/glextension.h>

PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;

//=================== BUFFER =========================
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;

PFNGLENABLEVERTEXATTRIBARRAYPROC	glEnableVertexAttribArray = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC		glVertexAttribPointer = nullptr;
PFNGLBINDVERTEXARRAYPROC			glBindVertexArray = nullptr;
PFNGLGENVERTEXARRAYSPROC			glGenVertexArrays = nullptr;
PFNGLCREATEVERTEXARRAYSPROC			glCreateVertexArrays = nullptr;
PFNGLVERTEXATTRIBDIVISORPROC		glVertexAttribDivisor = nullptr;
PFNGLDRAWARRAYSINSTANCEDPROC		glDrawArraysInstanced = nullptr;
PFNGLDRAWELEMENTSBASEVERTEXPROC     glDrawElementsBaseVertex = nullptr;

PFNGLDELETEBUFFERSPROC				glDeleteBuffers = nullptr;
PFNGLDELETEVERTEXARRAYSPROC			glDeleteVertexArrays = nullptr;

//================== PROGRAM ========================
PFNGLCREATEPROGRAMPROC			glCreateProgram		= nullptr;
PFNGLATTACHSHADERPROC			glAttachShader			= nullptr;
PFNGLLINKPROGRAMPROC			glLinkProgram		= nullptr;
PFNGLGETPROGRAMIVPROC			glGetProgramiv		= nullptr;
PFNGLDELETESHADERPROC			glDeleteShader			= nullptr;
PFNGLDELETEPROGRAMPROC			glDeleteProgram		= nullptr;
PFNGLUSEPROGRAMPROC				glUseProgram			= nullptr;
PFNGLCREATESHADERPROC			glCreateShader			= nullptr;
PFNGLSHADERSOURCEPROC			glShaderSource			= nullptr;
PFNGLCOMPILESHADERPROC			glCompileShader		= nullptr;
PFNGLGETSHADERIVPROC			glGetShaderiv		= nullptr;
PFNGLGETPROGRAMINFOLOGPROC		glGetProgramInfoLog	= nullptr;
PFNGLGETSHADERINFOLOGPROC		glGetShaderInfoLog		= nullptr;

//=================== TEXTURE ===========================

PFNGLACTIVETEXTUREPROC			glActiveTexture			= nullptr;
PFNGLCREATETEXTURESPROC			glCreateTextures		= nullptr;
PFNGLTEXTUREPARAMETERIPROC		glTextureParameteri		= nullptr;
PFNGLBINDTEXTUREUNITPROC		glBindTextureUnit		= nullptr;
PFNGLGENERATEMIPMAPPROC			glGenerateMipmap        = nullptr;
//PFNGLTEXSUBIMAGE2DEXTPROC		glTexSubImage2D = nullptr;		//overlab instead gl.h

//=================== FRAME BUFFER ======================
PFNGLGENFRAMEBUFFERSPROC			glGenFramebuffers = nullptr;
PFNGLBINDFRAMEBUFFERPROC			glBindFramebuffer = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC	glFramebufferTexture2D = nullptr;
PFNGLDRAWBUFFERSPROC				glDrawBuffers = nullptr;
PFNGLGENRENDERBUFFERSPROC		glGenRenderbuffers = nullptr;
PFNGLBINDRENDERBUFFERPROC		glBindRenderbuffer = nullptr;
PFNGLRENDERBUFFERSTORAGEPROC		glRenderbufferStorage = nullptr;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC	glCheckFramebufferStatus = nullptr;
PFNGLBLITFRAMEBUFFERPROC			glBlitFramebuffer = nullptr;

//=================== EXT ===============================
PFNGLNAMEDBUFFERSUBDATAEXTPROC				glNamedBufferSubDataEXT = nullptr;
PFNGLBINDBUFFERBASEPROC						glBindBufferBase        = nullptr;
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC			glCompressedTexImage2DARB = nullptr;
PFNGLBINDBUFFERRANGEPROC					glBindBufferRange = nullptr;
PFNGLDRAWELEMENTSINSTANCEDPROC				glDrawElementsInstanced = nullptr;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC	glDrawElementsInstancedBaseVertex = nullptr;

//================== GL 4.5 ==============================
PFNGLCLIPCONTROLPROC					glClipControl = nullptr;

namespace WGL
{
bool InitGLEXT();
bool InitWGL()
{
	//it's working while wglMakeCurrent(context, gldevicecontext)
	static bool isInitWGL = false;
	GL_DECL_PFN(PFNWGLSWAPINTERVALEXTPROC, wglSwapIntervalEXT);
	GL_DECL_PFN(PFNWGLCREATECONTEXTATTRIBSARBPROC, wglCreateContextAttribsARB);
	GL_DECL_PFN(PFNWGLCHOOSEPIXELFORMATARBPROC, wglChoosePixelFormatARB);
	isInitWGL = true;

	isInitWGL = InitGLEXT();

	return isInitWGL;
}

bool InitGLEXT()
{
	static bool isInitGLEXT = false;
	GL_DECL_PFN(PFNGLGENBUFFERSPROC, glGenBuffers);
	GL_DECL_PFN(PFNGLBINDBUFFERPROC, glBindBuffer);
	GL_DECL_PFN(PFNGLBUFFERDATAPROC, glBufferData);
	GL_DECL_PFN(PFNGLBUFFERSUBDATAPROC, glBufferSubData);

	GL_DECL_PFN(PFNGLENABLEVERTEXATTRIBARRAYPROC,	glEnableVertexAttribArray);
	GL_DECL_PFN(PFNGLVERTEXATTRIBPOINTERPROC,		glVertexAttribPointer);
	GL_DECL_PFN(PFNGLBINDVERTEXARRAYPROC,			glBindVertexArray);
	GL_DECL_PFN(PFNGLGENVERTEXARRAYSPROC,			glGenVertexArrays);
	GL_DECL_PFN(PFNGLCREATEVERTEXARRAYSPROC,		glCreateVertexArrays);
	GL_DECL_PFN(PFNGLVERTEXATTRIBDIVISORPROC,		glVertexAttribDivisor);
	GL_DECL_PFN(PFNGLDRAWARRAYSINSTANCEDPROC,		glDrawArraysInstanced);
	GL_DECL_PFN(PFNGLDRAWELEMENTSBASEVERTEXPROC,	glDrawElementsBaseVertex);

	GL_DECL_PFN(PFNGLDELETEBUFFERSPROC,				glDeleteBuffers);
	GL_DECL_PFN(PFNGLDELETEVERTEXARRAYSPROC,			glDeleteVertexArrays);

	GL_DECL_PFN(PFNGLCREATEPROGRAMPROC			,glCreateProgram);
	GL_DECL_PFN(PFNGLATTACHSHADERPROC			,glAttachShader);
	GL_DECL_PFN(PFNGLLINKPROGRAMPROC			,glLinkProgram);
	GL_DECL_PFN(PFNGLGETPROGRAMIVPROC			,glGetProgramiv);
	GL_DECL_PFN(PFNGLDELETESHADERPROC			,glDeleteShader);
	GL_DECL_PFN(PFNGLDELETEPROGRAMPROC			,glDeleteProgram);
	GL_DECL_PFN(PFNGLUSEPROGRAMPROC				,glUseProgram);
	GL_DECL_PFN(PFNGLCREATESHADERPROC			,glCreateShader);
	GL_DECL_PFN(PFNGLSHADERSOURCEPROC			,glShaderSource);
	GL_DECL_PFN(PFNGLCOMPILESHADERPROC			,glCompileShader);
	GL_DECL_PFN(PFNGLGETSHADERIVPROC			,glGetShaderiv);
	GL_DECL_PFN(PFNGLGETPROGRAMINFOLOGPROC		,glGetProgramInfoLog);
	GL_DECL_PFN(PFNGLGETSHADERINFOLOGPROC	    ,glGetShaderInfoLog);

	GL_DECL_PFN(PFNGLACTIVETEXTUREPROC			,glActiveTexture);
	GL_DECL_PFN(PFNGLCREATETEXTURESPROC			,glCreateTextures);
	GL_DECL_PFN(PFNGLTEXTUREPARAMETERIPROC		,glTextureParameteri);
	GL_DECL_PFN(PFNGLBINDTEXTUREUNITPROC		,glBindTextureUnit);
	GL_DECL_PFN(PFNGLGENERATEMIPMAPPROC			,glGenerateMipmap);
	//GL_DECL_PFN(PFNGLTEXSUBIMAGE2DEXTPROC		,glTexSubImage2D);

	GL_DECL_PFN(PFNGLGENFRAMEBUFFERSPROC			,glGenFramebuffers);
	GL_DECL_PFN(PFNGLBINDFRAMEBUFFERPROC			,glBindFramebuffer);
	GL_DECL_PFN(PFNGLFRAMEBUFFERTEXTURE2DPROC	,glFramebufferTexture2D);
	GL_DECL_PFN(PFNGLDRAWBUFFERSPROC				,glDrawBuffers);
	GL_DECL_PFN(PFNGLGENRENDERBUFFERSPROC		,glGenRenderbuffers);
	GL_DECL_PFN(PFNGLBINDRENDERBUFFERPROC		,glBindRenderbuffer);
	GL_DECL_PFN(PFNGLRENDERBUFFERSTORAGEPROC		,glRenderbufferStorage);
	GL_DECL_PFN(PFNGLFRAMEBUFFERRENDERBUFFERPROC ,glFramebufferRenderbuffer);
	GL_DECL_PFN(PFNGLCHECKFRAMEBUFFERSTATUSPROC	,glCheckFramebufferStatus);
	GL_DECL_PFN(PFNGLBLITFRAMEBUFFERPROC			,glBlitFramebuffer);
		
	GL_DECL_PFN(PFNGLNAMEDBUFFERSUBDATAEXTPROC	,glNamedBufferSubDataEXT);
	GL_DECL_PFN(PFNGLBINDBUFFERBASEPROC			,glBindBufferBase);
	GL_DECL_PFN(PFNGLCOMPRESSEDTEXIMAGE2DARBPROC,glCompressedTexImage2DARB);
	GL_DECL_PFN(PFNGLBINDBUFFERRANGEPROC		,glBindBufferRange);
	GL_DECL_PFN(PFNGLDRAWELEMENTSINSTANCEDPROC				,glDrawElementsInstanced);
	GL_DECL_PFN(PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC	,glDrawElementsInstancedBaseVertex);


	GL_DECL_PFN(PFNGLCLIPCONTROLPROC				,glClipControl);

	//Add More Function

	isInitGLEXT = true;
	return isInitGLEXT;
}
} // End of WGL

