#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Application
{
public:
	Application();
	~Application();
	void Run();

	//TODO: making layer system is a must before implementing IMGUI

private:
	GLFWwindow* m_Window;
};


Application* CreateApplication();
