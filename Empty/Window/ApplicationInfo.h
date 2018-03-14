#pragma once

#include <System/SubSystem.h>

struct Graphics
{
	bool antialiased = false;
};

struct DirInfo
{
	const std::string dxshader = "../data/shader/dx/";
	const std::string glshader = "../data/shader/gl/";
	const std::string model = "../data/model/";
	const std::string font = "../data/font/";
};


class ApplicationInfo : public SubSystem
{
public:
	ApplicationInfo() 
		: width(0),
		height(0),
		hwnd(nullptr),
		flags(0),
		vsync(true), 
		fullscreen(false)
	{}
	uint			width;
	uint			height;
	void*			hwnd;					//render native window handle
	GraphicAPI			apiType;
	bool			vsync;
	bool			fullscreen;
	//bool			enbleRenderDebug;

	Graphics		detail;
	//window style
	ApplicationFlag flags;

	std::string		appName = "Untitled Engine";
	std::string		renderWindowName = "none";
	std::string		version = "0.0.1";

	DirInfo			dir;

	//directx/vulkan is device based
	void* device = NULL;
	void* context = NULL;

	std::string GetApiString()
	{
		switch (apiType)
		{
		case GraphicAPI::DIRECTX11:
			return "DirectX 11.1";
		case GraphicAPI::OPENGL45:
			return "OpenGL 4.5";
		case GraphicAPI::VULKAN:
			return "Vulkan";
		default:
			return "Unknown";
		}
	}
};