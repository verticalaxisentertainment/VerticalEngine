#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Layer/LayerStack.h"

class Application
{
public:
	Application();
	~Application();
	void Run();

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);

	inline static Application& Get() { return *s_Instance; }

private:
	GLFWwindow* m_Window;
	LayerStack m_LayerStack;

	static Application* s_Instance;
};


Application* CreateApplication();
