#include "pch.h"
#include "Application.h"

#include <array>

Application* Application::s_Instance = nullptr;

#define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)

Application::Application()
{
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallBack(BIND_EVENT_FN(OnEvent));

    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    
    //Imgui Stuff
    m_ImGuiLayer = new ImGUILayer();
    PushLayer(m_ImGuiLayer);


    PushLayer(new ExampleLayer());
    PushLayer(new GameLayer());

    std::string vertexSrc=R"(
	#version 330 core

	layout (location=0) in vec3 a_Position;

    void main()
    {
		gl_Position=vec4(a_Position,1.0);
    }
	)";

    std::string fragmentSrc = R"(
	#version 330 core

	layout (location=0) out vec4 color;

    void main()
    {
		color=vec4(1.0,0.0,0.0,1.0);
    }
	)";

    m_Shader.reset(new Shader("src/shader.shader"));
}

Application::~Application()
{
}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

    for(auto it=m_LayerStack.end();it!=m_LayerStack.begin();)
    {
        (*--it)->OnEvent(e);
        if (e.Handled)
            break;
    }

    //std::cout << e << std::endl;
}


void Application::Run()
{
    
    while (m_Running)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.25f, 0.5f, 0, 1);

        m_Shader->Bind();
        for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack)
            layer->OnImGuiRender();
        m_ImGuiLayer->End();

        

        m_Window->OnUpdate();

    }

}

void Application::PushLayer(Layer* layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay)
{
    m_LayerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    m_Running = false;
    return true;
}
