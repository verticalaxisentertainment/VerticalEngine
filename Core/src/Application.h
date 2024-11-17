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
#include "Renderer/Texture.h"

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

	inline std::shared_ptr<FrameBuffer>& GetFrameBuffer() { return m_FrameBuffer; }

	ImGUILayer* m_ImGuiLayer;
	inline static bool showPostProcessing = false;
	std::shared_ptr<Texture2D> texture = nullptr;
	std::shared_ptr<FrameBuffer> m_IMGUIFrameBuffer;
private:
	bool OnWindowClose(WindowCloseEvent& e);
	bool OnWindowResize(WindowResizeEvent& e);
	bool OnMouseMoved(MouseMovedEvent& e);

	LayerStack m_LayerStack;
	int m_LayerCount;


	std::unique_ptr<Window> m_Window;
	std::shared_ptr<FrameBuffer> m_FrameBuffer;
		
	inline static bool m_Running = true;
	static Application* s_Instance;
};

Application* CreateApplication();//defined client side
