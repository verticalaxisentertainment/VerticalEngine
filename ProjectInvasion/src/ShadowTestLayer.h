#pragma once
#include "Layer/Layer.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Renderer/OrthographicCameraController.h"

class ShadowLayer:public Layer
{
public:
	ShadowLayer()
		:Layer("ShadowLayer")
	{
		m_Camera.reset(new OrthographicCameraController(1280.0f / 720.0f, false));
	}

	void OnUpdate() override
	{
		Renderer::BeginScene(m_Camera->GetCamera());
		Renderer::DrawTriangle({ 0.0f,0.0f,0.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		Renderer::EndScene();
	}

private:
	std::shared_ptr<OrthographicCameraController> m_Camera;
};
