#include "pch.h"
#include "Application.h"

#include <array>

#include "stb_image.h"
#include "Layer/GameLayer.h"
#include "Layer/DebugLayer.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallBack(BIND_EVENT_FN(Application::OnEvent));


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);

    //Imgui Stuff
    m_ImGuiLayer = new ImGUILayer();
    PushOverlay(m_ImGuiLayer);
    PushOverlay(new DebugLayer());

    PushLayer(new GameLayer());

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glm::mat4  projection;
    projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

    m_FrameBuffer.reset(FrameBuffer::Create());


    m_Texture.reset(Texture2D::Create("assets/textures/container.jpg"));
    m_Texture2.reset(Texture2D::Create("assets/textures/rifki.jpeg"));

    m_Shader.reset(new Shader("assets/shaders/shader.shader"));
    m_Shader->Bind();
    m_Shader->SetMat4("projection", projection);
    m_Shader->SetInt("tex", 1);
    m_Shader->SetInt("tex2", 2);

    m_ScreenShader.reset(new Shader("assets/shaders/screen.shader"));
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
        if (showPostProcessing)
        {
            m_FrameBuffer->Bind();
            glEnable(GL_DEPTH_TEST);
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3((m_mouseX - GetWindow().GetWidth()/2) / GetWindow().GetWidth()*2*2, (-m_mouseY + GetWindow().GetHeight()/2) / GetWindow().GetHeight()*2*1.5f, 0.0f));

        m_Texture->Bind(1);
        m_Texture2->Bind(2);

        m_Shader->Bind();
        m_Shader->SetFloat("iTime", glfwGetTime());
        m_Shader->SetMat4("model", model);
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