#include "pch.h"
#include "Application.h"

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
    PushOverlay(m_ImGuiLayer);

    PushLayer(new ExampleLayer());

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
        glViewport(0, 0, 800, 600);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.25f, 0.5f, 0, 1);

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
