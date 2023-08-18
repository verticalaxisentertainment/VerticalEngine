#include "pch.h"
#include "UIRenderer.h"
#include "Renderer.h"
#include "Application.h"
#include "Input.h"
#include "UUID.h"

#include <ft2build.h>
#include FT_FREETYPE_H

static float* s_PixelData;

void UI::SetPixelData(float* pixelData)
{
	s_PixelData = pixelData;
}

float* UI::GetPixelData()
{
	return s_PixelData;
}



//---------------------------------- QUAD ---------------------------------------------------
UIQuad::UIQuad()
{
	int r = (id::UUID().id() & 0x000000FF) >> 0;
	int g = (id::UUID().id() & 0x0000FF00) >> 8;
	int b = (id::UUID().id() & 0x00FF0000) >> 16;

	m_ID = r + g + b;
}

UIQuad::~UIQuad() 
{
	
}

void UIQuad::Render(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
{
	
	Renderer::DrawQuad(position, scale, color, m_ID);
	m_Color = color;
}

bool UIQuad::IsClicked()
{
	if (IsHovered())
	{
		if (Input::IsMouseButtonPressed(Mouse::Button0))
			return true;
	}
	return false;
}

bool UIQuad::IsHovered()
{
	Application& app = Application::Get();
	int pickedID = app.GetFrameBuffer()->ReadPixelInt(2, Input::GetLocalMouseX(), app.GetWindow().GetHeight() - Input::GetLocalMouseY());
	
	if (pickedID == m_ID)
		return true;
	return false;
}
//---------------------------------------------------------------------------------------------------



//---------------------------------------CIRCLE------------------------------------------------------
UICircle::UICircle()
{
	int r = (id::UUID().id() & 0x000000FF) >> 0;
	int g = (id::UUID().id() & 0x0000FF00) >> 8;
	int b = (id::UUID().id() & 0x00FF0000) >> 16;

	m_ID = r + g + b;
}

UICircle::~UICircle() 
{
}

void UICircle::Render(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
{
	glm::mat4 obj(1.0f);
	obj = glm::translate(obj, position);
	obj = glm::scale(obj, glm::vec3(scale, 1.0f));
	Renderer::DrawCircle(obj, color, 1.0f, 0.005f, m_ID);
}

bool UICircle::IsClicked()
{
	if (IsHovered())
	{
		if (Input::IsMouseButtonPressed(Mouse::Button0))
			return true;
	}
	return false;
}

bool UICircle::IsHovered()
{
	Application& app = Application::Get();
	int pickedID = app.GetFrameBuffer()->ReadPixelInt(2, Input::GetLocalMouseX(), app.GetWindow().GetHeight() - Input::GetLocalMouseY());


	if (pickedID == m_ID)
		return true;
	return false;
}
//---------------------------------------------------------------------------------------------------
