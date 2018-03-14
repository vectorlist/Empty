#include <PCH.h>

#include <window/application.h>
#include <window/applicationinfo.h>

#include <cache/scenecache.h>

#include <Scene/FontBatchScene.h>
#include <Scene/ImageScene.h>

#define TEST_ASSERT(x, ...) \
do{							\
}while(false)


int main(int args, char* argv[])
{
	auto info = AddSubSystem<ApplicationInfo>();
	info->width = 1280;
	info->height = 720;
	info->appName = "Empty Engine ";
	info->apiType = GraphicAPI::OPENGL45;
	//info->apiType = GraphicAPI::DIRECTX11;
	info->flags = APP_CENTERPOS | APP_TITLEBAR;
	info->vsync = false;

	Application app;
	
	G_SceneCache->AddScene<FontBatchScene>("font");
	G_SceneCache->AddScene<ImageScene>("image");
	G_SceneCache->SetQueueScene("image");


	return app.Run();
}