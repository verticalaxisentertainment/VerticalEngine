#ifdef PLATFORM_WINDOWS

extern Application* CreateApplication();

int main(int argc, char** argv)
{
	auto app = CreateApplication();
	app->Run();
	delete app;
}
#endif
#ifdef PLATFORM_ANDROID
#include <jni.h>
#include <android/log.h>

extern "C"
JNI_EXPORT void JNICALL Java_com_as_example_main_1activity_hello_1cpp(JNIEnv* env,jobject thisObject)
{
	__android_log_write(ANDROID_LOG_INFO, "CPP", "oh hai!, I'm c++");
}
#endif


