#include "DLL.h"
#include "GameLayer.h"
#include "DebugLayer.h"
#include "SceneInit.h"
#include "TestLayer.h"

class Engine :public Application
{
public:
	Engine()
	{
		PushLayer(new GameLayer());
		//PushLayer(new SceneInit());
		PushOverlay(new DebugLayer());
		//PushLayer(new TestLayer());


	}
	~Engine()
	{
		
	}
};


Application* CreateApplication()
{
	return new Engine();
}