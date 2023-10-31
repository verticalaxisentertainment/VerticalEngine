#include "pch.h"
#include "UIRenderer.h"
#include "Renderer.h"
#include "Application.h"
#include "Input.h"
#include "UUID.h"

#include "RenderCommand.h"

#define BIND_EVENT_FN(x) std::bind(&x,std::placeholders::_1)

int UIRenderer::GetPickedID()
{
	Application& app = Application::Get();
	int pickedID = app.GetFrameBuffer()->ReadPixelInt(2, Input::GetLocalMouseX(), app.GetWindow().GetHeight() - Input::GetLocalMouseY());
	return pickedID;
}

void UIRenderer::OnEvent(Event& e)
{
	EventDispatcher dispathcer(e);

	dispathcer.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(UIRenderer::OnMouseClicked));
	dispathcer.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(UIRenderer::OnMouseReleased));
	dispathcer.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(UIRenderer::OnWindowResize));
}

bool UIRenderer::IsHovered(const int& ID)
{
	Application& app = Application::Get();
	int pickedID = app.GetFrameBuffer()->ReadPixelInt(2, Input::GetLocalMouseX(), app.GetWindow().GetHeight() - Input::GetLocalMouseY());

	INFO(m_onUI);

	if (pickedID == ID)
	{
		m_onUI = true;
		return true;
	}

	m_onUI = false;
	return false;
	
}

bool UIRenderer::IsClicked(const int& ID)
{
	if (IsHovered(ID))
	{
		if (m_MouseClicked)
		{
			//m_MouseClicked = false;
			return true;
		}
	}
	return false;
}

bool UIRenderer::OnMouseClicked(MouseButtonPressedEvent& e)
{
	m_MouseClicked = true;
	if (m_onUI)
		return true;
	else
		return false;
}

bool UIRenderer::OnMouseReleased(MouseButtonReleasedEvent& e)
{
	m_MouseClicked = false;
	return true;
}

bool UIRenderer::OnWindowResize(WindowResizeEvent& e)
{
	Renderer::GetSceneData()->UIViewProjectionMatrix = glm::ortho(-static_cast<float>(e.GetWidth()) / 2, static_cast<float>(e.GetWidth()) / 2, -static_cast<float>(e.GetHeight()) / 2, static_cast<float>(e.GetHeight()) / 2);
	//auto test = glm::clamp((int)e.GetWidth() / 10, -100, 100);
	//INFO("Resized {}", test);
	return false;
}
