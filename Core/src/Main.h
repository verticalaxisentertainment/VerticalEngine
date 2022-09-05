#ifdef PLATFORM_WINDOWS

extern Application* CreateApplication();

int main(int argc, char** argv)
{
	auto app = CreateApplication();
	app->Run();
	delete app;
}
#endif



