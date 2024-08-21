#include "DLL.h"
#include "GameLayer.h"

class Engine :public Application
{
public:
	Engine()
	{
	}
	~Engine()
	{
		
	}
};


Application* CreateApplication()
{
	return new Engine();
}