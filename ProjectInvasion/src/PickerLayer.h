#pragma once
#include "Layer/Layer.h"
#include "Renderer/Renderer.h"
#include "GameLayer.h"

#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

class PickerLayer :public Layer
{
public:
	void OnUpdate() override
	{
		m_X = Input::GetLocalMouseX();
		m_Y = Input::GetLocalMouseY();

		Renderer::BeginScene(GameLayer::m_CameraController->GetCamera());
		glm::vec3 test = Math::ScreenToWorldPoint(glm::vec3(m_X, m_Y, 1.0f), GameLayer::m_CameraController->GetCamera().GetViewProjectionMatrix());

		if (GameLayer::isBox)
		{
			Renderer::DrawQuad({ test.x,test.y ,0.1f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,0.5f });
		}
		else
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, glm::vec3(test.x, test.y, 0.1f));
			Renderer::DrawCircle(model, { 1.0f,1.0f,1.0f,0.5f });
		}
		Renderer::EndScene();
	}

	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
		GameLayer::m_CameraController->OnEvent(e);

		if (!e.Handled && e.IsInCategory(EventCategoryMouse))
		{
			//dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(PickerLayer::onMouseMoved));
			dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(PickerLayer::onMouseClicked));
		}
	}

private:
	float m_X, m_Y;

	/*bool onMouseMoved(MouseMovedEvent& e)
	{
		Application& app = Application::Get();
		m_X = e.GetX();
		m_Y = e.GetY();
		return true;
	}*/

	bool onMouseClicked(MouseButtonPressedEvent& e)
	{
		if (!GameLayer::onUI)
		{
			if (e.GetMouseButton() == Mouse::Button0)
			{
				translate = true;

				if (GameLayer::isBox)
				{
					glm::vec3 position = Math::ScreenToWorldPoint(glm::vec3(m_X, m_Y, 1.0f), GameLayer::m_CameraController->GetCamera().GetViewProjectionMatrix());
					Physics::CreateDynamicBox({ position.x,position.y,1.0f }, { 1.0f,1.0f });
					return true;
				}
				else
				{
					glm::vec3 position = Math::ScreenToWorldPoint(glm::vec3(m_X, m_Y, 1.0f), GameLayer::m_CameraController->GetCamera().GetViewProjectionMatrix());
					Physics::CreateDynamicCircle({ position.x,position.y,1.0f }, 1.0f);
					return true;
				}
			}
		}
		return false;
	}
};
