#include "pch.h"
#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "box2d/b2_fixture.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"
#include "Input.h"
#include "Scene/Scene.h"

#include "optick.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
    s_Instance = this;
    m_Window = std::unique_ptr<Window>(Window::Create({ "Project Invasion | Renderer: " + RendererAPI::GetAPIString() + " | GPU: " }));
    m_Window->SetEventCallBack(BIND_EVENT_FN(Application::OnEvent));

#ifdef MOBILE
    //Win32Menu::OpenMenu();
    if(gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
    {
        INFO("Mobile Support ON!");
    }
    
#else

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        ERROR("Failed to initialize GLAD");
    }
#endif

    Log::Init();
    INFO("{} | {}", (char*)fmt::ptr(glGetString(GL_VERSION)), (char*)fmt::ptr(glGetString(GL_RENDERER)));

    //Imgui Stuff
    m_ImGuiLayer = new ImGUILayer();
    PushOverlay(m_ImGuiLayer);


    RenderCommand::Init();
    Renderer::Init();

    FrameBufferSpecification spec;
    spec.Width = m_Window->GetWidth();
    spec.Height = m_Window->GetHeight();
    m_FrameBuffer.reset(FrameBuffer::Create(spec));

    GetWindow().GetTitle() = "Project Invasion | Renderer: " + RendererAPI::GetAPIString() + " | GPU: " + (const char*)glGetString(GL_RENDERER);
}

Application::~Application()
{
}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
    dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(Application::OnMouseMoved));

    for(auto it=m_LayerStack.end();it!=m_LayerStack.begin();)
    {
        (*--it)->OnEvent(e);
        if (e.Handled)
            break;
    }
}


void Application::Run()
{
    while (m_Running)
    {
        OPTICK_FRAME("Main Thread");

        Renderer::ResetStats();

        glEnable(GL_DEPTH_TEST);
        if (showPostProcessing)
        {
            m_FrameBuffer->Bind();
        }
		
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (Layer* layer : m_LayerStack)
        {
            layer->OnUpdate();
        }

        if(showPostProcessing)
        {
	        m_FrameBuffer->UnBind();
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            Renderer::DrawFrameBuffer(m_FrameBuffer);
        }

        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack)
        {
            layer->OnImGuiRender();
        }
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

void Application::PopLayer(Layer* layer)
{
    m_LayerStack.PopLayer(layer);
    layer->OnDetach();
}

void Application::PopOverlay(Layer* overlay)
{
    m_LayerStack.PopLayer(overlay);
    overlay->OnDetach();
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
    m_FrameBuffer->Resize(e.GetWidth(), e.GetHeight());
    return false;
}

bool Application::OnMouseMoved(MouseMovedEvent& e)
{
    return false;
}
