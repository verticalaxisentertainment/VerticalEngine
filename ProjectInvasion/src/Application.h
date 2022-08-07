#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Layer/LayerStack.h"
#include "Layer/ImGUILayer.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"


class ExampleLayer:public Layer
{
public:
	ExampleLayer()
		:Layer("ExampleLayer"){}

	void OnUpdate() override
	{
		std::cout << "ExampleLayer::Update\n";
	}

	void OnEvent(Event& e) override
	{
		std::cout << e << "\n";
	}
};

class ExampleLayer2 :public Layer
{
public:
	ExampleLayer2()
		:Layer("ExampleLayer2") {}

	void OnUpdate() override
	{
		std::cout << "ExampleLayer::Update\n";
	}

	void OnEvent(Event& e) override
	{
		std::cout << e << "\n";
	}
};


class Application
{
public:
	Application();
	~Application();
	void Run();

	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);

	inline static Application& Get() { return *s_Instance; }
	inline Window& GetWindow() const{ return *m_Window; }

private:
	bool OnWindowClose(WindowCloseEvent& e);
	LayerStack m_LayerStack;

	ImGUILayer* m_ImGuiLayer;

	std::unique_ptr<Window> m_Window;
	bool m_Running = true;

	static Application* s_Instance;
};

Application* CreateApplication();
