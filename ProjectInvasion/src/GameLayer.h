#pragma once
#include "Layer/Layer.h"
#include "Events/KeyEvent.h"
#include "Renderer/Renderer.h"
#include "Math/Mathematics.h"
#include "KeyCode.h"
#include "MouseCode.h"
#include "Renderer/RenderCommand.h"

#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

bool translate = false;

class GameLayer :public Layer
{
public:
	GameLayer()
		:Layer("GameLayer")
	{
		m_CameraController.reset(new OrthographicCameraController(1280.0f / 720.0f, true));
		m_Texture.reset(Texture2D::Create("assets/textures/container.jpg"));
		m_TextureTest.reset(Texture2D::Create("assets/textures/red.jpg"));
		m_newTexture.reset(Texture2D::Create("assets/textures/rifki.jpeg"));

		Physics::CreateStaticBody({ 0.0f,0.0f,1.0f }, { 50.0f,1.0f });
	}

	void OnAttach() override
	{
		FrameBufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer.reset(FrameBuffer::Create(fbSpec));
	}

	void OnUpdate() override
	{
		float time = Math::Time();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		auto& pos = m_CameraController->GetCamera().GetPosition();
		RenderCommand::Clear();
		if (!m_LockCamera)
		{
			m_CameraController->SetPosition(pos);
			m_CameraController->OnUpdate(timestep);
		}

		Renderer::BeginScene(m_CameraController->GetCamera());
		Application& app = Application::Get();

		glm::vec3 test = Math::ScreenToWorldPoint(glm::vec3(m_X, m_Y, 1.0f), m_CameraController->GetCamera().GetViewProjectionMatrix());

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 5.0f, -0.1f));

		Renderer::DrawQuad(model, m_Texture);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 5, 0.5f));
		//model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));
		Renderer::DrawQuad(model, m_TextureTest);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 5, 0.5f));
		Renderer::DrawQuad(model, m_newTexture);

		if (m_LockCamera)
		{
			pos.x = Math::Lerp(pos.x, Physics::GetLastObjectsPos().x, (float)timestep*2);
			pos.y = Math::Lerp(pos.y, Physics::GetLastObjectsPos().y, (float)timestep*2);
			m_CameraController->GetCamera().SetPosition(pos);
		}

		//Renderer::DrawLine({ test.x,test.y,0.5f }, {0.0f,5.0f,0.5f}, { 1.0f,1.0f,1.0f,1.0f });

		if (isBox)
		{
			Renderer::DrawQuad({ test.x,test.y ,0.1f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,0.5f });

		}
		else
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(test.x, test.y, 0.1f));
			Renderer::DrawCircle(model, { 1.0f,1.0f,1.0f,0.5f });
		}

		for (int i = 0; i < tiles[0]; i++)
		{
			for (int k = 0; k < tiles[1]; k++)
				Renderer::DrawQuad({ 5.0f + i * 1.5f,3.0f + k * 1.5f,-0.1f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		}
		
		Physics::Simulate(timestep);

		//Renderer::RenderText("Select a shape", { 0.0f,0.0f }, 0.5f, { 1.0f,1.0f,1.0f,1.0f });


		Renderer::DrawLine({ glm::sin(Math::Time()) / 2,-glm::cos(Math::Time()) / 2,1.0f }, { 0.0f,0.0f,1.0f }, { 0.0f,0.0f,1.0f,1.0f });
		Renderer::DrawLine({ -0.5f,0.0f,1.0f }, { 0.5f,0.0f,1.0f }, { 1.0f,0.0,0.0f,1.0f });
		Renderer::DrawLine({ 0.0f,-0.5f,1.0f }, { 0.0f,0.5f,1.0f }, { 0.0f,1.0,0.0f,1.0f });

		Renderer::EndScene();
	}

	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
		m_CameraController->OnEvent(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(GameLayer::onKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(GameLayer::onKeyReleased));
		if(!e.Handled&&e.IsInCategory(EventCategoryMouse))
		{
			dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(GameLayer::onMouseMoved));
			dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(GameLayer::onMouseClicked));
		}
	}

	inline static int tiles[2];
	static std::shared_ptr<Texture2D> m_Texture;
	static std::shared_ptr<OrthographicCameraController> m_CameraController;
	static std::shared_ptr<FrameBuffer> m_FrameBuffer;
	inline static bool onUI = false, isBox = true, m_LockCamera = false;
private:
	std::shared_ptr<Texture2D> m_newTexture;
	std::shared_ptr<Texture2D> m_TextureTest;
	std::vector<glm::vec2> positions;
	float m_X, m_Y;
	float m_LastFrameTime;
	float* pixelData;

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

	bool onMouseClicked(MouseButtonPressedEvent& e)
	{
		if(!onUI)
		{
			if (e.GetMouseButton() == Mouse::Button0)
			{
				translate = true;

				if(isBox)
				{
					glm::vec3 position = Math::ScreenToWorldPoint(glm::vec3(m_X, m_Y, 1.0f), m_CameraController->GetCamera().GetViewProjectionMatrix());
					Physics::CreateDynamicBox({ position.x,position.y,1.0f }, { 1.0f,1.0f });
					return true;
				}
				else
				{
					glm::vec3 position = Math::ScreenToWorldPoint(glm::vec3(m_X, m_Y, 1.0f), m_CameraController->GetCamera().GetViewProjectionMatrix());
					Physics::CreateDynamicCircle({ position.x,position.y,1.0f }, 1.0f);
					return true;
				}
			}
		}
		return false;
	}
};

std::shared_ptr<Texture2D> GameLayer::m_Texture;
std::shared_ptr<OrthographicCameraController> GameLayer::m_CameraController;
std::shared_ptr<FrameBuffer> GameLayer::m_FrameBuffer;