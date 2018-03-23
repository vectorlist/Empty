#include <PCH.h>

#include <window/application.h>
#include <window/applicationinfo.h>

#include <cache/scenecache.h>

#include <Scene/FontBatchScene.h>
#include <Scene/ImageScene.h>
#include <Scene/SkyScene.h>

int main(int args, char* argv[])
{
	auto info = AddSubSystem<ApplicationInfo>();
	info->width = 1280;
	info->height = 720;
	info->appName = "Empty Engine ";
	info->apiType = GraphicAPI::OPENGL45;
	//info->apiType = GraphicAPI::DIRECTX11;
	info->flags = APP_CENTERPOS | APP_TITLEBAR;
	info->vsync = true;

	Application app;
	
	G_SceneCache->AddScene<SkyScene>("SkyDome");
	G_SceneCache->AddScene<FontBatchScene>("Font");
	G_SceneCache->AddScene<ImageScene>("Image");
	G_SceneCache->SetQueueScene("SkyDome");


	return app.Run();
}