#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Layer/LayerStack.h"
#include "Layer/ImGUILayer.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "Renderer/Shader.h"
#include <array>

class ExampleLayer:public Layer
{
public:
	ExampleLayer()
		:Layer("ExampleLayer"){}

	void OnUpdate() override
	{
		//std::cout << "ExampleLayer::Update\n";
	}

	void OnEvent(Event& e) override
	{
		std::cout << e << "\n";
	}
};

class GameLayer :public Layer
{
public:
	GameLayer()
		:Layer("GameLayer")
	{
		vertices = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f
		};

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glad_glEnableVertexAttribArray(0);
	}

	void OnUpdate() override
	{
		glBindVertexArray(VBO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void OnEvent(Event& e) override
	{
		e.Handled |= e.IsInCategory(EventCategoryMouse);
	}
private:
	std::array<float, 10> vertices;
	unsigned int VBO;
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

	std::unique_ptr<Shader> m_Shader;

	static Application* s_Instance;
};

Application* CreateApplication();
