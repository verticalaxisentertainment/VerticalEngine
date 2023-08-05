#pragma once
#include "Layer/Layer.h"
#include "Renderer/Renderer.h"
#include "Renderer/UIRenderer.h"

#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

class TestLayer:public Layer
{
public:
	TestLayer()
		:Layer("TestLayer") {
	}

	void OnUpdate() override
	{
		Application& app = Application::Get();
		float windowWidth = app.GetWindow().GetWidth();
		float windowHeight = app.GetWindow().GetHeight();
		float aspect = windowWidth / windowHeight;

		Renderer::BeginScene();
		Renderer::DrawQuad({ windowWidth - 75.0f * aspect,windowHeight - 40.0f * aspect,0.5f }, { 150.0f * aspect,80.0f * aspect }, { 0.38f,0.38f,0.38f,0.5f });
		selectQuad.Render({ windowWidth - 100.0f * aspect,windowHeight - 40.0f * aspect,1.0f }, { 50.0f * aspect,50.0f * aspect }, m_ColorBox);
		selectCircle.Render({ windowWidth - 35.0f * aspect,windowHeight - 40.0f * aspect,1.0f }, { 50.0f * aspect,50.0f * aspect }, m_ColorCircle);

		//Renderer::RenderText("Select a shape", { windowWidth * aspect, windowHeight * aspect }, 500.f * aspect, { 1.0f,1.0f,1.0f,1.0f });
		Renderer::RenderText("Select a shape", { 0.0f,0.0f }, 0.5f * aspect, { 1.0f,1.0f,1.0f,1.0f });


		Renderer::EndScene();


		if (selectQuad.IsHovered() || selectCircle.IsHovered())
		{
			GameLayer::onUI = true;
			app.GetWindow().SetCursor(Cursor::HAND);
		}
		else
		{
			GameLayer::onUI = false;
			app.GetWindow().SetCursor(Cursor::ARROW);
		}

		if (selectQuad.IsClicked())
		{
			m_ColorBox = { 0.0f,0.0f,0.0f,1.0f }; GameLayer::isBox = true;
		}
		else if (selectCircle.IsClicked())
		{
			m_ColorCircle = { 0.0f,0.0f,0.0f,1.0f }; GameLayer::isBox = false;
		}
		else
		{
			m_ColorCircle = { 0.0f,1.0f,0.0f,1.0f };
			m_ColorBox = { 1.0f,0.0f,0.0f,1.0f };
		}
	}

	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
	}

private:
	UIQuad selectQuad;
	UICircle selectCircle;
	float* pixelData;
	glm::vec4 m_ColorBox= { 1.0f,0.0f,0.0f,1.0f }, m_ColorCircle= { 0.0f,1.0f,0.0f,1.0f };
	float m_x, m_y;

};