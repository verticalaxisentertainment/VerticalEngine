#pragma once
#include "Layer/Layer.h"
#include "Events/KeyEvent.h"
#include "Renderer/Renderer.h"

#include <glad/glad.h>

#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)



class GameLayer :public Layer
{
public:
	GameLayer()
		:Layer("GameLayer")
	{
		m_Texture.reset(Texture2D::Create("assets/textures/container.jpg"));
	}

	void OnUpdate() override
	{
		Application& app = Application::Get();
		Renderer::DrawQuad({ 0.0f,0.0f }, { 1.0f,2.0f }, m_Texture);
		Renderer::DrawQuad({ (m_X - app.GetWindow().GetWidth() / 2) / app.GetWindow().GetWidth() * 2 * 2,0.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });

	}

	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(GameLayer::onKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(GameLayer::onKeyReleased));
		if(!e.Handled&&e.IsInCategory(EventCategoryMouse))
		{
			dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(GameLayer::onMouseMoved));
		}
	}


	static std::shared_ptr<Texture2D> m_Texture;
private:
	float m_X, m_Y;


	bool onKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == GLFW_KEY_C)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		return true;
	}

	bool onKeyReleased(KeyReleasedEvent& e)
	{
		if (e.GetKeyCode() == GLFW_KEY_C)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
