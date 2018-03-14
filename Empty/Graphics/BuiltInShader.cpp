#include <PCH.h>
#include <graphics/builtinshader.h>
#include <graphics/context.h>

//======================================= SCENE =======================================
namespace glsl
{
	const char* sceneVS = R"(
#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 st;

layout(std140, row_major, binding = 0) uniform UBO
{
	mat4 proj;
	mat4 view;
	mat4 model;
};

out Frag{
	vec3 normal;
	vec3 pos;
	vec2 st;
}frag;

//uniform mat4 model;

void main()
{
	vec4 world = model * vec4(pos, 1);
	gl_Position = proj * view * world;
	frag.normal = (model * vec4(normal,0)).xyz;
	frag.pos = (model*vec4(pos,1)).xyz;
	frag.st = st;
};
)";

	const char* sceneFS = R"(
#version 450 core

uniform sampler2D tex01;

in Frag{
	vec3 normal;
	vec3 pos;
	vec2 st;
}frag;

out vec4 outColor;

void main()
{
    vec3 lightDir = vec3(30,30,30) - vec3(0,0,0);
	float dist = length(lightDir);

	lightDir = normalize(lightDir);
	
	float diff = dot(lightDir, frag.normal);
	diff = (diff + 1) * 0.5;
	
	float toon;
	
	float interval = 1.0 / 4.0;
	toon = floor(diff/interval) * interval;
	toon = max(0.3, toon);
	
	vec3 diffuse = texture(tex01, frag.st).rgb; 
	diffuse = diffuse * toon;

	outColor = vec4(diffuse ,1);
};	
)";

//======================================= AABB =======================================
	const char* aabbVS = R"(
#version 450 core

layout (location = 0) in vec3 bmin;
layout (location = 1) in vec3 bmax;
layout (location = 2) in vec3 color;
layout (location = 3) in mat4 world;

//need only view porj
layout(std140, row_major, binding = 0) uniform UBO
{
	mat4 proj;
	mat4 view;
	mat4 model;		//dont use
};

//             ( 1, 1, 1)
//    5 -------- 6
//   /          /
//  1 -------- 2 |
//  |          | |
//  | 4        | 7
//  |/         |/
//  0 -------- 3
//(-1,-1, -1)
//
// Vertice = 8
// Draw Order Vertice 8(front) + 8(back) + 8(side) == 24 

out PS
{
	vec3 color;
}ps;

void GetWorldVertices(in vec3 src[8], out vec3 dst[8])
{
	//test
	for(int i =0; i < 8; i++)
	{
		dst[i] = (transpose(world) * vec4(src[i],1)).xyz; 

		//dst[i] = src[i];
	}
};

void main()
{
	//gl_VertexID is ordering gldrawarray or instancing N verices[N] index
	
	//SOURCE VTX	
	const vec3 srcVtx[8] ={
	//FRONT
	{bmin.x, bmin.y, bmin.z},			//0					
	{bmin.x, bmax.y, bmin.z},			//1
	{bmax.x, bmax.y, bmin.z},			//2
	{bmax.x, bmin.y, bmin.z},			//3
	//BACK
	{bmin.x, bmin.y, bmax.z},			//4					
	{bmin.x, bmax.y, bmax.z},			//5
	{bmax.x, bmax.y, bmax.z},			//6
	{bmax.x, bmin.y, bmax.z}			//7
	};
	
	//PREPARE WORLD VTX
	vec3 worldVtx[8];		
	GetWorldVertices(srcVtx, worldVtx);

	//REFINE MIN MAX
	vec3 dmin = worldVtx[0];
	vec3 dmax = worldVtx[0];

	for(int i =1; i < 8; ++i)
	{
		dmin = min(dmin, worldVtx[i]);
		dmax = max(dmax, worldVtx[i]);
	}	

    //TRANSFORMED VTX
	const vec3 vtx[8] ={
	//FRONT
	{dmin.x, dmin.y, dmin.z},			//0					
	{dmin.x, dmax.y, dmin.z},			//1
	{dmax.x, dmax.y, dmin.z},			//2
	{dmax.x, dmin.y, dmin.z},			//3
	//BACK
	{dmin.x, dmin.y, dmax.z},			//4					
	{dmin.x, dmax.y, dmax.z},			//5
	{dmax.x, dmax.y, dmax.z},			//6
	{dmax.x, dmin.y, dmax.z}			//7
	};

	//DRAW LINE ORDER INDEX (0 - 1, 1 - 2...)
	const vec3 drawVtx[24] = vec3[24](
	vtx[0], vtx[1], vtx[1], vtx[2], vtx[2], vtx[3], vtx[3], vtx[0],  //FRONT
	vtx[4], vtx[5], vtx[5], vtx[6], vtx[6], vtx[7], vtx[7], vtx[4],  //BACK
	vtx[0], vtx[4], vtx[1], vtx[5], vtx[2], vtx[6], vtx[3], vtx[7]  //SIDE
	);
	
	//GET CURRENT VERTEX
	vec3 curVtx = drawVtx[gl_VertexID];
	vec4 position = view * vec4(curVtx, 1);
	gl_Position = proj * position;
	ps.color = color;
};

)";

	const char* aabbFS = R"(
#version 450 core

in PS
{
	vec3 color;
}ps;

out vec4 outColor;
void main()
{
	outColor = vec4(ps.color,1);
};

)";

//======================================= FONT =======================================
	const char* instanceFontVS = R"(
#version 450 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 uv;
layout(location = 2) in vec4 color;

//Draw Instance Arrays at once [6] times

layout(std140, row_major, binding = 1) uniform UBO  //avoid main uniform buffer(binding = 0)
{
	mat4 screen;
}ubo;

//  x y    x + w, y
//	|		/ |
//	|	   /  |
//	|	  /   |
//	|    /    |
//	|	/     |
//	|  /      |
//  x ,y + h  x + w, y + h

//per instancing index 0 1 2 2 1 3

out struct PS
{
	vec2 st;
	vec4 color;
}ps;

void main()
{
	const vec3 vtx[6]= {
	{pos.x        , pos.y		 , 0},		// 0 index
	{pos.x + pos.z, pos.y        , 0},		// 1 index
	{pos.x        , pos.y + pos.w, 0},		// 2 index
	{pos.x        , pos.y + pos.w, 0},		// 2 index
	{pos.x + pos.z, pos.y        , 0},		// 1 index
	{pos.x + pos.z, pos.y + pos.w, 0},		// 3 index
	};

	const vec2 st[6] = {
	{uv.x, uv.y},
	{uv.z, uv.y},
	{uv.x, uv.w},
	{uv.x, uv.w},
	{uv.z, uv.y},
	{uv.z, uv.w}
	};
	
	vec3 currentVtx = vtx[gl_VertexID];
	vec2 currentST = st[gl_VertexID];
	gl_Position = ubo.screen * vec4(currentVtx,1);
	ps.st = currentST;
	ps.color = color;
};
	
)";
	const char* instanceFontFS = R"(
#version 450 core

uniform sampler2D tex01;

out vec4 finalColor;

in struct PS
{
	vec2 st;
	vec4 color;
}ps;

void main()
{
	vec4 fontColor = vec4(1,1,1, texture(tex01, ps.st).r);
	//input color has alpha
	finalColor = fontColor * ps.color;
};

)";

}//end of glsl





//=========================== HLSL ==================================

namespace hlsl
{
	const char* sceneVS = R"(
cbuffer matrice : register(b0)
{
	matrix proj;
	matrix view;
	matrix model;
};

struct VS_In
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 st : ST;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
};

struct PS_In
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 st : ST;
};

PS_In VS(VS_In vs)
{
	PS_In ps;
	ps.pos.w = 1;
	ps.pos = mul(vs.pos, model);
	ps.pos = mul(ps.pos, view);
	ps.pos = mul(ps.pos, proj);

	ps.normal = mul(vs.normal, (float3x3)model);
	ps.st = vs.st;
	//ps.normal = vs.normal;
	return ps;
};
)";

	const char* sceneFS = R"(
Texture2D tex0;
SamplerState samplerState;

struct PS_In
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 st : ST;
};

float4 PS(PS_In ps) : SV_TARGET
{
	float3 diffuse = tex0.Sample(samplerState, ps.st).rgb;
	float3 lightDir = float3(3,3,3) - float3(0,0,0);
	float dist = length(lightDir);

	float delta = dot(ps.normal, normalize(lightDir));
	delta = (delta + 1) * 0.5;

	float interval = 1.0 / 4.0;
	
	float toon = 0;
	toon = floor(delta/ interval) * interval;
	toon = max(0.3, toon);

	diffuse = toon * diffuse;
	
	return float4(diffuse,1);
};

)";

}

void DXInternalSceneShader::CreateInputLayout(ID3DBlob* vBlob)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0 ,							0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "ST",		  0, DXGI_FORMAT_R32G32_FLOAT,    0 , D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	LOG_HR << G_DXDevice->CreateInputLayout(
		layout.data(),
		layout.size(),
		vBlob->GetBufferPointer(),
		vBlob->GetBufferSize(),
		&mInputLaout);
}

namespace hlsl
{
	const char* aabbVS = R"(
cbuffer matrice : register(b0)
{
	matrix proj;
	matrix view;
	matrix model;
};

struct VS_In
{
	//float4 pos : POSITION;
	float3 bmin : POSITION;
	float3 bmax : TEXCOORD;
	float3 color : COLOR;
	float4x4 world : TRANSFORM;
	uint id : SV_VertexID;
};

struct PS_In
{
	float4 pos : SV_POSITION;
	float4 color : TEXCOORD;
};

//void GetWorldVertices(float3 src[8], float3 dst[8], float4x4 world)
//{
//	for(int i =0; i < 8; i++)
//	{
//		dst[i] = mul(world,float4(src[i],1)).xyz; 
//	}
//};

PS_In VS(VS_In vs)
{
	PS_In ps;
	//vs.pos.w =1;
	float3 bmin = vs.bmin.xyz;
	float3 bmax = vs.bmax.xyz;
	
	const float3 srcVtx[8] ={
	{bmin.x, bmin.y, bmin.z},			//0					
	{bmin.x, bmax.y, bmin.z},			//1
	{bmax.x, bmax.y, bmin.z},			//2
	{bmax.x, bmin.y, bmin.z},			//3
	{bmin.x, bmin.y, bmax.z},			//4					
	{bmin.x, bmax.y, bmax.z},			//5
	{bmax.x, bmax.y, bmax.z},			//6
	{bmax.x, bmin.y, bmax.z}			//7
	};
	
	//PREPARE WORLD VTX
	float3 worldVtx[8];		
	//GetWorldVertices(srcVtx, worldVtx, vs.world);
	
	for(int i =0; i <8; i++)
	{
		worldVtx[i] = mul(float4(srcVtx[i],1),vs.world).xyz;
	}

	//REFINE MIN MAX
	float3 dmin = worldVtx[0];
	float3 dmax = worldVtx[0];

	for(int i =1; i < 8; ++i)
	{
		dmin = min(dmin, worldVtx[i]);
		dmax = max(dmax, worldVtx[i]);
	}	

    //TRANSFORMED VTX
	const float3 vtx[8] ={
	//FRONT
	{dmin.x, dmin.y, dmin.z},			//0					
	{dmin.x, dmax.y, dmin.z},			//1
	{dmax.x, dmax.y, dmin.z},			//2
	{dmax.x, dmin.y, dmin.z},			//3
	//BACK
	{dmin.x, dmin.y, dmax.z},			//4					
	{dmin.x, dmax.y, dmax.z},			//5
	{dmax.x, dmax.y, dmax.z},			//6
	{dmax.x, dmin.y, dmax.z}			//7
	};

	//DRAW LINE ORDER INDEX (0 - 1, 1 - 2...)
	float3 drawVtx[24] = {
	vtx[0], vtx[1], vtx[1], vtx[2], vtx[2], vtx[3], vtx[3], vtx[0],  //FRONT
	vtx[4], vtx[5], vtx[5], vtx[6], vtx[6], vtx[7], vtx[7], vtx[4],  //BACK
	vtx[0], vtx[4], vtx[1], vtx[5], vtx[2], vtx[6], vtx[3], vtx[7]  //SIDE
	};

	float4 curPos = float4(drawVtx[vs.id],1);
	//ps.pos = mul(curPos, vs.world);
	ps.pos = curPos;
	ps.pos = mul(ps.pos, view);
	ps.pos = mul(ps.pos, proj);
	ps.color = float4(vs.color,1);
	return ps;
};

)";
	const char* aabbFS = R"(
struct PS_In
{
	float4 pos : SV_POSITION;
	float4 color : TEXCOORD;
};

float4 PS(PS_In ps) : SV_TARGET
{
	//return float4(1,0,0,1);
	return ps.color;
};

)";
}

void DXInternalAABBShader::CreateInputLayout(ID3DBlob* vBlob)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputs = {
		//{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_INSTANCE_DATA,1 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA,1 },
		{ "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA,1 },
		{ "TRANSFORM",0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA,1 },
		{ "TRANSFORM",1,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA,1 },
		{ "TRANSFORM",2,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA,1 },
		{ "TRANSFORM",3,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA,1 }
	};

	LOG_HR << G_DXDevice->CreateInputLayout(
		inputs.data(),
		inputs.size(),
		vBlob->GetBufferPointer(),
		vBlob->GetBufferSize(),
		&mInputLaout);
}

namespace hlsl
{
const char* instanceFontVS = R"(
cbuffer matrice : register(b1)				//index 0 avoid main matrices
{
	matrix screen;
};

struct VS_In
{
	float4 bpos : POSITION;
	float4 buv : TEXCOORD;
	float4 color : COLOR;
};

struct PS_In
{
	float4 pos : SV_POSITION;
	float2 st : TEXCOORD;
	float4 color : COLOR;
};	

PS_In VS(VS_In vs, uint id : SV_VertexID)
{
	//Relative GLSL Instancing
	PS_In ps;
	
	float4 pos = vs.bpos;
	float4 uv = vs.buv;
	
	const float3 vtx[6]= {
	{pos.x        , pos.y		 , 0},		// 0 index
	{pos.x + pos.z, pos.y        , 0},		// 1 index
	{pos.x        , pos.y + pos.w, 0},		// 2 index
	{pos.x        , pos.y + pos.w, 0},		// 2 index
	{pos.x + pos.z, pos.y        , 0},		// 1 index
	{pos.x + pos.z, pos.y + pos.w, 0}		// 3 index
	};

	const float2 st[6] = {
	{uv.x, uv.y},
	{uv.z, uv.y},
	{uv.x, uv.w},
	{uv.x, uv.w},
	{uv.z, uv.y},
	{uv.z, uv.w}
	};

	float3 currentVtx = vtx[id];
	float2 currentST = st[id];

	ps.pos = mul(float4(currentVtx,1), screen);
	ps.st = currentST;
	ps.color = vs.color;
	return ps;
};


)";
const char* instanceFontFS = R"(
struct PS_In
{
	float4 pos : SV_POSITION;
	float2 st : TEXCOORD;
	float4 color : COLOR;
};

Texture2D tex0;
SamplerState samplerState;

float4 PS(PS_In ps) : SV_TARGET
{
	float4 color = ps.color;
	float4 texColor = tex0.Sample(samplerState, ps.st);
	color.a = texColor.r;
	return color;
};

)";

}

void DXInternalFontShader::CreateInputLayout(ID3DBlob* vBlob)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputs = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_INSTANCE_DATA,1 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA,1 },
		{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA,1 }
	};

	G_DXDevice->CreateInputLayout(
		inputs.data(),
		inputs.size(),
		vBlob->GetBufferPointer(),
		vBlob->GetBufferSize(),
		&mInputLaout);
}
