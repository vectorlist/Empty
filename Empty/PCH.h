#pragma once

#define NOMINMAX
#undef min
#undef max

#include <algorithm>

#define SAFE_RELEASE(x) if(x){ x->Release(); x = nullptr;}
#define SAFE_DELETE(x)  if(x){ delete x; x = nullptr;}

#ifdef _DEBUG
#define ASSERT(expr) \
    do { \
        if (!(expr)) { \
           _CrtDbgBreak(); \
        } \
    } while (false)
#else
#define ASSERT(expr) do { } while (false)
#endif

#ifdef _DEBUG
#define ASSERT_MSG(expr, msg) \
    do { \
        if (!(expr)) { \
			_CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, nullptr, msg); \
        } \
    } while (false)
#else
#define ASSERT_MSG(expr, msg) do { } while (false)
#endif


#define FINAL			final
#define OVERRIDE		override
#define FORCEINLINE		__forceinline
#define INLINE			inline

typedef unsigned int	uint;
typedef unsigned char	uint8;

//STD
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <map>
#include <assert.h>

//STATIC MATH
#include <math/vec2f.h>
#include <math/vec3f.h>
#include <math/matrix4x4.h>
#include <math/math.h>
#include <math/aabb.h>

//ETC
#include <core/ForwardDecl.h>
#include <core/type.h>
#include <core/log.h>

//Defines
//=================== FONT =======================
#define FONT_MAX_BATCH						65536
#define FONT_MAX_CHAR						128
#define FONT_MIN_CHAR						32
