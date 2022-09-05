#include "DLL.h"

class SandBox:public Application
{
public:
	SandBox()
	{

	}
	~SandBox()
	{
		
	}
};


Application* CreateApplication()
{
	return new SandBox();
}