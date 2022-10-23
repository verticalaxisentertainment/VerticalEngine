#include "pch.h"
#include "WindowsWindow.h"

#include "Events/KeyEvent.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include <glad/glad.h>

static bool s_GLFWInitialized = false;
static void GLFWErrorCallback(int error,const char* description)
{
	ERROR(error << ": " << description);
}

Window* Window::Create(const WindowProps& props)
{
	return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props)
{
	Init(props);
	m_Cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
}

WindowsWindow::~WindowsWindow()
{
	Shutdown();
}

void WindowsWindow::OnUpdate()
{
	glViewport(0, 0, GetWidth(), GetHeight());

	glfwSetCursor(m_Window, m_Cursor);

	glfwPollEvents();
	glfwSwapBuffers(m_Window);

}

void WindowsWindow::SetCursor(Cursor cursor)
{
	if (cursor == Cursor::HAND)
		m_Cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	if (cursor == Cursor::ARROW)
		m_Cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
}

void WindowsWindow::SetVSync(bool enabled)
{
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	m_Data.VSync = enabled;
}

bool WindowsWindow::IsVSync() const
{
	return m_Data.VSync;
}

void WindowsWindow::Init(const WindowProps& props)
{
	m_Data.Title = props.Title;
	m_Data.Height = props.Height;
	m_Data.Width = props.Width;

	if(!s_GLFWInitialized)
	{
		int success = glfwInit();
		glfwSetErrorCallback(GLFWErrorCallback);
		s_GLFWInitialized = true;
	}

	m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, &m_Data);
	SetVSync(true);

	//set GLFW callbacks
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
}

void WindowsWindow::Shutdown()
{
	glfwDestroyWindow(m_Window);
}
