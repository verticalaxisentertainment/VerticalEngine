#pragma once
#include "Layer/Layer.h"
#include "Renderer/Renderer.h"


#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

class TestLayer:public Layer
{
public:
	TestLayer()
		:Layer("TestLayer"){}

	void OnUpdate() override
	{
		Application& app = Application::Get();

		Renderer::BeginScene();
			Renderer::DrawQuad({ 1.55f,0.9f,0.5f }, { 0.5f,0.2f }, { 0.38f,0.38f,0.38f,0.5f });
			Renderer::DrawQuad({ 1.45f,0.9f,1.0f }, { 0.1f,0.1f }, m_ColorBox);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(1.65f, 0.9f, 1.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			Renderer::DrawCircle(model, m_ColorCircle);

			Renderer::DrawLine({ glm::sin(Math::Time()) / 4 - 1.5f,-glm::cos(Math::Time())/4-0.75f,1.0f }, { -1.5f,-0.75f,1.0f }, { 0.0f,0.0f,1.0f,1.0f });
			Renderer::DrawLine({ -1.75f,-0.75f,1.0f }, { -1.25f,-0.75f,1.0f }, { 1.0f,0.0,0.0f,1.0f });
			Renderer::DrawLine({ -1.5f,-0.5f,1.0f }, { -1.5f,-1.0f,1.0f }, { 0.0f,1.0,0.0f,1.0f });
		Renderer::EndScene();

		pixelData = pixelData = app.GetFrameBuffer()->ReadPixel(0, m_x, app.GetWindow().GetHeight() - m_y);

		if ((pixelData[0] == 1&&pixelData[2]==0))
		{
			app.GetWindow().SetCursor(Cursor::HAND);
		}
		else if((pixelData[1] == 1 && pixelData[0] == 0))
		{
			app.GetWindow().SetCursor(Cursor::HAND);
		}
		else
		{
			app.GetWindow().SetCursor(Cursor::ARROW);
		}
	}

	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(TestLayer::onMouseMoved));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(TestLayer::onMouseClicked));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(TestLayer::onMouseReleased));
	}

private:
	float* pixelData;
	glm::vec4 m_ColorBox= { 1.0f,0.0f,0.0f,1.0f }, m_ColorCircle= { 0.0f,1.0f,0.0f,1.0f };
	float m_x, m_y;
	bool onMouseMoved(MouseMovedEvent& e)
	{
		m_x = e.GetX();
		m_y = e.GetY();
		return false;
	}

	bool onMouseClicked(MouseButtonPressedEvent& e)
	{
		if(e.GetMouseButton()==Mouse::Button0)
		{
			if((pixelData[1] == 1 && pixelData[0] == 0))
			{
				GameLayer::onUI = true;
				GameLayer::isBox = false;
				m_ColorCircle = { 0.0f,0.0f,0.0f,1.0f };
			}
			else if ((pixelData[0] == 1 && pixelData[2] == 0))
			{
				GameLayer::onUI = true;
				GameLayer::isBox = true;
				m_ColorBox = { 0.0f,0.0f,0.0f,1.0f };
			}
			else
			{
				GameLayer::onUI = false;
			}
		}
		return false;
	}

	bool onMouseReleased(MouseButtonReleasedEvent& e)
	{
		if(e.GetMouseButton()==Mouse::Button0)
		{
			m_ColorBox = { 1.0f,0.0f,0.0f,1.0f };
			m_ColorCircle = { 0.0f,1.0f,0.0f,1.0f };
		}
		return false;
	}
};