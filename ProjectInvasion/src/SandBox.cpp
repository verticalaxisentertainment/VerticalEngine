#include "DLL.h"
#include "GameLayer.h"
#include "DebugLayer.h"

class SandBox:public Application
{
public:
	SandBox()
	{
		PushLayer(new GameLayer());
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