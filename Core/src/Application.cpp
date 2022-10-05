#include "pch.h"
#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "box2d/b2_fixture.h"
#include "Renderer/Renderer.h"
//#include "Layer/DebugLayer.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
    s_Instance = this;
    m_Window = std::unique_ptr<Window>(Window::Create({"Project Invasion | Renderer: " +RendererAPI::GetAPIString()+" | GPU: " }));
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

    INFO(glGetString(GL_VERSION) <<" | " << glGetString(GL_RENDERER));

    //Imgui Stuff
    m_ImGuiLayer = new ImGUILayer();
    PushOverlay(m_ImGuiLayer);
    //PushOverlay(new DebugLayer());


    Renderer::Init();
    //PushLayer(new GameLayer());


    m_FrameBuffer.reset(FrameBuffer::Create());
    m_ScreenShader.reset(new Shader("assets/shaders/screen.glsl"));
    m_ScreenShader->Bind();
    m_ScreenShader->SetInt("screenTexture", 0);
}

Application::~Application()
{
}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

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
        
        std::string title = "Project Invasion | Renderer: " + RendererAPI::GetAPIString() + " | GPU: "+(const char*)glGetString(GL_RENDERER);
        glfwSetWindowTitle(static_cast<GLFWwindow*>(GetWindow().GetNativeWindow()), title.c_str());
        
        if (showPostProcessing)
        {
            m_FrameBuffer->Bind();
            glEnable(GL_DEPTH_TEST);
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (Layer* layer : m_LayerStack)
            layer->OnUpdate();


        if(showPostProcessing)
        {
	        m_FrameBuffer->UnBind();
	        glDisable(GL_DEPTH_TEST);
	        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	        glClear(GL_COLOR_BUFFER_BIT);

	        m_ScreenShader->Bind();
	        m_ScreenShader->SetVec2("resoulation", glm::vec2(m_Window->GetWidth(), m_Window->GetHeight()));
	        m_ScreenShader->SetFloat("iTime", glfwGetTime());
	        m_FrameBuffer->BindVertexArray();
	        glDrawElements(GL_TRIANGLES, m_FrameBuffer->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        }

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