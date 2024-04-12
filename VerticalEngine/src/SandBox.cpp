#include "DLL.h"
#include "GameLayer.h"
#include "DebugLayer.h"
#include "TestLayer.h"
#include "SceneInit.h"

class SandBox :public Application
{
public:
	SandBox()
	{
		PushLayer(new GameLayer());
		//PushLayer(new TestLayer());
		//PushLayer(new SceneInit());
		PushOverlay(new DebugLayer());

	}
	~SandBox()
	{
		
	}
};


Application* CreateApplication()
{
	return new SandBox();
}