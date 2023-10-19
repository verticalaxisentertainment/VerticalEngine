#include "DLL.h"
#include "GameLayer.h"
#include "DebugLayer.h"
#include "TestLayer.h"
#include "ShadowTestLayer.h"
#include "PickerLayer.h"

class SandBox :public Application
{
public:
	SandBox()
	{
		PushLayer(new GameLayer());
		//PushLayer(new ShadowLayer());
		PushLayer(new TestLayer());
		//PushLayer(new PickerLayer());
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