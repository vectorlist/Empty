#pragma once

//=========== Forward Decl ========================

class Object;
class Model;
class Shader;
class Camera;
class Context;
class Input;
class Scene;
class SceneCache;
class Buffer;
class Texture;
class GLTexture;
class DXTexture;
class UniformBuffer;

//============ BATCH / RENDERER ===================
class DebugBatch;
class FontBatch;

//============ PHYSICS ============================
class BVHTreeNode;

//============ OpenGL4 Foward Decalre =============
struct HWND__;
typedef HWND__ *HWND;
struct HDC__;
typedef HDC__ *HDC;
struct HGLRC__;
typedef HGLRC__ *HGLRC;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned int GLenum;

//========== DirectX11 Foward Decalre =============
struct ID3D11Device;
struct ID3D11DeviceContext;
