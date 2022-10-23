#include "DLL.h"
#include "GameLayer.h"
#include "DebugLayer.h"
#include "TestLayer.h"

class SandBox:public Application
{
public:
	SandBox()
	{
		PushLayer(new GameLayer());
		PushLayer(new TestLayer());
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