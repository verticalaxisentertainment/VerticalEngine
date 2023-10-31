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
		m_Scene.reset(new Scene("Sandbox"));
		m_CameraController.reset(new OrthographicCameraController(1280.0f / 720.0f, true));
	}

	void OnAttach() override
	{
		SceneSerializer::Read(m_Scene->m_SceneName, m_Scene);
		Application::Get().GetWindow().GetTitle() += " | Scene: " + m_Scene->m_SceneName;
	}

	void OnUpdate() override
	{
		float time = Math::Time();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		m_CameraController->OnUpdate(timestep);

		RenderCommand::Clear();

		//m_Scene->RenderScene(m_CameraController->GetCamera());
		DebugLayer::scenes[DebugLayer::selectedScene]->RenderScene(m_CameraController->GetCamera());

		DebugLayer::scenes[DebugLayer::selectedScene]->Simulate(timestep);
	}

	void OnEvent(Event& e) override
	{
		m_CameraController->OnEvent(e);
	}

private:
	std::shared_ptr<Scene> m_Scene;
	std::shared_ptr<OrthographicCameraController> m_CameraController;
	float m_LastFrameTime = 0.0f;
};
