#pragma once
#include "Layer/Layer.h"
#include "Events/KeyEvent.h"
#include "Renderer/Renderer.h"
#include "Input.h"
#include "KeyCode.h"

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

	float x=0.0f,y=0.0f;
	void OnUpdate() override
	{
		if(Input::IsKeyPressed(Key::Right))
		{
			x += 0.01f;
		}
		else if(Input::IsKeyPressed(Key::Left))
		{
			x -= 0.01f;
		}
		if(Input::IsKeyPressed(Key::Up))
		{
			y += 0.01f;
		}
		else if(Input::IsKeyPressed(Key::Down))
		{
			y -= 0.01f;
		}
		Application& app = Application::Get();
		glm::vec3 position(x, y, 1.0f);
		float currentX = m_X / app.GetWindow().GetWidth() * 2 - 1.0f;
		glm::vec4 tmp(currentX, 0, 0, 1);
		tmp = tmp * glm::inverse(app.m_CameraController.GetCamera().GetViewProjectionMatrix());
		currentX = tmp.x;

		//INFO(currentX);
		glm::mat4 model=glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(0, -1.0f, 0));
		//INFO(glm::radians(90.0f));


		Renderer::DrawQuad(model, m_Texture);
		/*Renderer::DrawQuad({ (m_X - app.GetWindow().GetWidth() / 2) / app.GetWindow().GetWidth() * 2.5f,0.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		Renderer::DrawQuad({ -1.0f,1.5f }, { 1.0f,1.0f }, { 1.0,1.0f,1.0f,1.0f });*/

		Renderer::DrawLine({glm::sin(glfwGetTime()),-glm::cos(glfwGetTime()),1.0f}, { 0.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f,1.0f });
		/*Renderer::DrawLine({ 0.0f,-1.0f,0.0f }, { 0.0f,1.0f,0.0f }, { 1.0f,0.0f,0.0f,1.0f });*/
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
