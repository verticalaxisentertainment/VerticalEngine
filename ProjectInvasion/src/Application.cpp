#include "pch.h"
#include "Application.h"

Application::Application()
{
    GLFWwindow* window;


    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(800, 600, "Project Invasion", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    m_Window = window;

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glfwSwapInterval(1);


   
}

Application::~Application()
{
}

void Application::Run()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        glViewport(0, 0, 800, 600);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.25f, 0.5f, 0, 1);

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    glfwDestroyWindow(m_Window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
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
