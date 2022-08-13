#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Layer/LayerStack.h"
#include "Layer/ImGUILayer.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Renderer/Shader.h"
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Renderer/Buffer.h"
#include <stb_image.h>
#include "Renderer/VertexArray.h"

#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

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
		//std::cout << e << "\n";
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

	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Shader> m_ScreenShader;
	std::shared_ptr<FrameBuffer> m_FrameBuffer;
	unsigned int framebuffer;
	unsigned int quadVAO, quadVBO;
	unsigned int textureColorbuffer;
	unsigned int rbo;


	static Application* s_Instance;
};

Application* CreateApplication();
