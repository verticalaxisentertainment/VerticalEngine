#pragma once

#include "Layer/Layer.h"
#include "Scene/Scene.h"
#include "Scene/SceneSerializer.h"

class SceneInit :public Layer
{
public:
	SceneInit()
		:Layer("SceneInit")
	{
		m_CameraController.reset(new OrthographicCameraController(1280.0f / 720.0f, true));
	}

	void OnAttach() override
	{
	}

	void OnUpdate() override
	{
		glm::vec3 worldCursorPos = Math::ScreenToWorldPoint(glm::vec3(Input::GetLocalMouseX(), Input::GetLocalMouseY(), 1.0f), m_CameraController->GetCamera().GetViewProjectionMatrix());

		float time = Math::Time();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		m_CameraController->OnUpdate(timestep);

		if (DebugLayer::m_Picker)
			DebugLayer::m_Picker.GetComponent<TransformComponent>().Translation = { worldCursorPos.x,worldCursorPos.y,0.5f };

		RenderCommand::Clear();

		//m_Scene->RenderScene(m_CameraController->GetCamera());
		DebugLayer::scenes[DebugLayer::selectedScene]->RenderScene(m_CameraController->GetCamera());

		if (DebugLayer::m_Quad && DebugLayer::m_Circle)
		{

			if (DebugLayer::m_Quad.IsHovered()||DebugLayer::m_Circle.IsHovered())
				Application::Get().GetWindow().SetCursor(Cursor::HAND);
			else
				Application::Get().GetWindow().SetCursor(Cursor::ARROW);
		}
		DebugLayer::scenes[DebugLayer::selectedScene]->Simulate(timestep);

	}

	void OnEvent(Event& e) override
	{
		m_CameraController->OnEvent(e);
		DebugLayer::scenes[DebugLayer::selectedScene]->OnEvent(e);
		UIRenderer::OnEvent(e);
	}
	
private:
	std::shared_ptr<OrthographicCameraController> m_CameraController;
	float m_LastFrameTime = 0.0f;
};
