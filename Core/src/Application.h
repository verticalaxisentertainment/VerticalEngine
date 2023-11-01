#pragma once

#include "Layer/LayerStack.h"
#include "Layer/ImGUILayer.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

#include "Renderer/FrameBuffer.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Core.h"
#include "Input.h"

#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)


class _API Application
{
public:
	Application();
	~Application();
	void Run();

	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);

	void PopLayer(Layer* layer);
	void PopOverlay(Layer* overlay);

	inline static Application& Get() { return *s_Instance; }
	inline Window& GetWindow() const{ return *m_Window; }

	inline const std::shared_ptr<FrameBuffer>& GetFrameBuffer() const { return m_FrameBuffer; }

	ImGUILayer* m_ImGuiLayer;
	inline static bool showPostProcessing = true;
private:
	bool OnWindowClose(WindowCloseEvent& e);
	bool OnWindowResize(WindowResizeEvent& e);
	bool OnMouseMoved(MouseMovedEvent& e);

	LayerStack m_LayerStack;
	int m_LayerCount;


	std::shared_ptr<FrameBuffer> m_FrameBuffer;
	std::unique_ptr<Window> m_Window;
	inline static bool m_Running = true;
	static Application* s_Instance;
};

Application* CreateApplication();//defined client side
