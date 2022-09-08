#include "DLL.h"

class SandBox:public Application
{
public:
	SandBox()
	{
		INFO("Application Created");
	}
	~SandBox()
	{
		
	}
};


Application* CreateApplication()
{
	return new SandBox();
}