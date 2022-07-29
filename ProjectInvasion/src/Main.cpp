#ifdef PLATFORM_WINDOWS
#include "Application.h"

int main()
{
	Application* app=new Application;
	app->Run();
	delete app;
}
#endif



