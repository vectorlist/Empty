#include <Core/PCH.h>

//#define _PROFILE

#include <window/application.h>
#include <window/applicationinfo.h>

#include <cache/scenecache.h>

#include <Scene/DeferredScene.h>
#include <Scene/CullScene.h>

int main(int args, char* argv[])
{
	auto info = AddSubSystem<ApplicationInfo>();
	info->width = 1280;
	info->height = 720;
	info->appName = "Empty Engine ";
	//info->apiType = GraphicAPI::OPENGL45;
	info->apiType = GraphicAPI::DIRECTX11;
	info->flags = APP_CENTERPOS | APP_TITLEBAR;
	info->vsync = false;

	Application app;
	
	SceneCache::AddScene<DeferredScene>("deferred");
	SceneCache::AddScene<CullScene>("cull");
	SceneCache::SetQueueScene("deferred");

	return app.Run();
}