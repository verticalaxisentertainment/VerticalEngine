#pragma once
#include "Layer/Layer.h"
#include "Renderer/Renderer.h"
#include "Renderer/UIRenderer.h"
#include <string>

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

		Renderer::BeginScene(GameLayer::m_CameraController->GetCamera());
		Renderer::DrawQuad({ windowWidth - 75.0f * aspect,windowHeight - 40.0f * aspect,0.5f }, { 150.0f * aspect,80.0f * aspect }, { 0.38f,0.38f,0.38f,0.5f });

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100.0f, 100.0f, 1.0f));
		model = glm::scale(model, glm::vec3(160.0f, 90.0f, 1.0f));
		//Renderer::DrawQuad(model, GameLayer::m_Texture);

		Renderer::RenderText("Select a shape", { windowWidth/2 - 140.0f * aspect, windowHeight - 100.0f * aspect,1.0f }, 0.4f * aspect, { 1.0f,1.0f,1.0f,1.0f });



		Renderer::EndScene();

		/*if (selectQuad.IsHovered() || selectCircle.IsHovered())
		{
			GameLayer::onUI = true;
			app.GetWindow().SetCursor(Cursor::HAND);
		}
		else
		{
			GameLayer::onUI = false;
			app.GetWindow().SetCursor(Cursor::ARROW);
		}*/
	}

	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
	}

	static char m_Text[128];
private:
	float* pixelData;
	glm::vec4 m_ColorBox= { 1.0f,0.0f,0.0f,1.0f }, m_ColorCircle= { 0.0f,1.0f,0.0f,1.0f };
	float m_x, m_y;
};

char TestLayer::m_Text[128];

