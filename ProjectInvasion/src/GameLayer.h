#pragma once
#include "Layer/Layer.h"
#include "Events/KeyEvent.h"
#include "Renderer/Renderer.h"
#include "Math/Mathematics.h"
#include "KeyCode.h"

#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)



class GameLayer :public Layer
{
public:
	GameLayer()
		:Layer("GameLayer"), m_CameraController(1280.0f / 720.0f, true)
	{
		m_Texture.reset(Texture2D::Create("assets/textures/container.jpg"));
		tiles[0] = 1;
		tiles[1] = 1;
	}

	void OnUpdate() override
	{
		float time = Math::Time();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		m_CameraController.OnUpdate(timestep);
		Renderer::BeginScene(m_CameraController.GetCamera());

		Application& app = Application::Get();

		glm::vec3 test = Math::ScreenToWorldPoint(glm::vec3(m_X, m_Y, 1.0f), m_CameraController.GetCamera().GetViewProjectionMatrix());

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)Math::Time(), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(0, -1.0f, 0));

		Renderer::DrawQuad({ test.x,test.y ,-0.5f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		Renderer::DrawQuad(model, m_Texture);

		for (int i = 0; i < tiles[0]; i++)
		{
			for (int k = 0; k < tiles[1]; k++)
				Renderer::DrawQuad({ 5.0f + i * 1.5f,3.0f + k * 1.5f,1.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		}

		Renderer::DrawLine({ glm::sin(Math::Time()),-glm::cos(Math::Time()),1.0f }, { 0.0f,0.0f,1.0f }, { 0.0f,0.0f,1.0f,1.0f });
		Renderer::DrawLine({ -1.0f,0.0f,1.0f }, { 1.0f,0.0f,1.0f }, { 1.0f,0.0,0.0f,1.0f });
		Renderer::DrawLine({ 0.0f,1.0f,1.0f }, { 0.0f,-1.0f,1.0f }, { 0.0f,1.0,0.0f,1.0f });
		/*Renderer::DrawLine({ 0.0f,-1.0f,0.0f }, { 0.0f,1.0f,0.0f }, { 1.0f,0.0f,0.0f,1.0f });*/

	}

	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
		m_CameraController.OnEvent(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(GameLayer::onKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(GameLayer::onKeyReleased));
		if(!e.Handled&&e.IsInCategory(EventCategoryMouse))
		{
			dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(GameLayer::onMouseMoved));
		}
	}

	inline static int tiles[2];
	static std::shared_ptr<Texture2D> m_Texture;
private:
	std::vector<glm::vec2> positions;
	OrthographicCameraController m_CameraController;
	float m_X, m_Y;
	float m_LastFrameTime;

	bool onKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::C)
		{
			Renderer::WireframeMode(true);
		}

		return true;
	}

	bool onKeyReleased(KeyReleasedEvent& e)
	{
		if (e.GetKeyCode() == Key::C)
		{
			Renderer::WireframeMode(false);
		}

		return true;
	}

	bool onMouseMoved(MouseMovedEvent& e)
	{
		Application& app = Application::Get();
		m_X = e.GetX();
		m_Y = e.GetY();
		return true;
	}
};

std::shared_ptr<Texture2D> GameLayer::m_Texture;