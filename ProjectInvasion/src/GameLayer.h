#pragma once
#include "Layer/Layer.h"
#include "Events/KeyEvent.h"
#include "Renderer/Renderer.h"
#include "Math/Mathematics.h"
#include "KeyCode.h"
#include "MouseCode.h"
#include "Renderer/RenderCommand.h"

#include "Scene/Scene.h"
#include "Scene/Entity.h"

#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

bool translate = false;

class GameLayer :public Layer
{
public:
	GameLayer()
		:Layer("GameLayer")
	{
		std::string startText = "Lorem Upsulum";
		for (int i = 0; i < startText.size(); i++)
			m_Text[i] = startText[i];

		m_CameraController.reset(new OrthographicCameraController(1280.0f / 720.0f, true));
		m_Texture.reset(Texture2D::Create("assets/textures/container.jpg"));
		m_TextureTest.reset(Texture2D::Create("assets/textures/red.jpg"));
		m_newTexture.reset(Texture2D::Create("assets/textures/rifki.jpeg"));
		m_ActiveScene.reset(new Scene());

		Physics::CreateStaticBody({ 0.0f,0.0f,1.0f }, { 50.0f,1.0f });
	}

	void OnAttach() override
	{
		FrameBufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer.reset(FrameBuffer::Create(fbSpec));

		Entity entity;
		entity = m_ActiveScene->CreateEntity();

		entity.GetComponent<TransformComponent>().Translation = { 0.0f,15.0f,0.0f };
		entity.AddComponent<BoxCollider2DComponent>();
		entity.AddComponent<RigidBody2DComponent>().BodyType = RigidBody2DComponent::Type::Dynamic;
		entity.AddComponent<SpriteRendererComponent>().Color = { 1.0f,0.0f,0.0f,0.5 };

		entity = m_ActiveScene->CreateEntity();
		entity.GetComponent<TransformComponent>().Translation = { 0.0f,0.0f,0.0f };
		entity.GetComponent<TransformComponent>().Scale = { 20.0f,1.0f,1.0f };
		entity.AddComponent<BoxCollider2DComponent>();
		entity.AddComponent<RigidBody2DComponent>().BodyType = RigidBody2DComponent::Type::Static;
		entity.AddComponent<SpriteRendererComponent>().Color = { 0.2f,0.2f,0.2f,0.5f };

		//m_ActiveScene->OnPhysics2DStart();
	}


	void OnUpdate() override
	{
		Application& app = Application::Get();
		m_X = Input::GetLocalMouseX();
		m_Y = Input::GetLocalMouseY();

		float time = Math::Time();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		auto& pos = m_CameraController->GetCamera().GetPosition();
		RenderCommand::Clear();
		if (!m_LockCamera && m_Move)
		{
			m_CameraController->SetPosition(pos);
			m_CameraController->OnUpdate(timestep);
		}


		Renderer::BeginScene(m_CameraController->GetCamera());

		auto view = m_ActiveScene->m_Registry.view<TransformComponent>();
		
		for (auto e : view)
		{
			Entity entity = { e,m_ActiveScene.get() };

			auto transform = entity.GetComponent<TransformComponent>();
			glm::vec4 Color = { 1.0f,1.0f,1.0f,1.0f };
			if(entity.HasComponent<SpriteRendererComponent>())
				Color = entity.GetComponent<SpriteRendererComponent>().Color;

			if(entity.HasComponent<BoxCollider2DComponent>())
				Renderer::DrawQuad(transform.GetTranform(), Color);
			if(entity.HasComponent<CircleCollider2DComponent>())
				Renderer::DrawCircle(transform.GetTranform(), Color);
		}

		glm::vec3 test = Math::ScreenToWorldPoint(glm::vec3(m_X, m_Y, 1.0f), m_CameraController->GetCamera().GetViewProjectionMatrix());

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 5.0f, -0.1f));

		Renderer::DrawQuad(model, m_Texture);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 5, 0.0f));
		//model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));
		Renderer::DrawQuad(model, m_TextureTest);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 5, 0.0f));
		Renderer::DrawQuad(model, m_newTexture);

		if (m_LockCamera)
		{
			pos.x = Math::Lerp(pos.x, Physics::GetLastObjectsPos().x, (float)timestep * 2);
			pos.y = Math::Lerp(pos.y, Physics::GetLastObjectsPos().y, (float)timestep * 2);
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


		Renderer::DrawLine({ glm::sin(Math::Time()) / 2,-glm::cos(Math::Time()) / 2,1.0f }, { 0.0f,0.0f,1.0f }, { 0.0f,0.0f,1.0f,1.0f });
		Renderer::DrawLine({ -0.5f,0.0f,1.0f }, { 0.5f,0.0f,1.0f }, { 1.0f,0.0,0.0f,1.0f });
		Renderer::DrawLine({ 0.0f,-0.5f,1.0f }, { 0.0f,0.5f,1.0f }, { 0.0f,1.0,0.0f,1.0f });

		Renderer::RenderText(m_Text, { -5.0f,2.0f,-0.8f }, 0.01f, { 0.0f,1.0f,0.5f,1.0f });

		Renderer::EndScene();
		m_ActiveScene->Simulate(timestep);
	}

	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
		if (!e.Handled)
		{
			m_Move = true;
			m_CameraController->OnEvent(e);
			//m_CameraController->CanMove(true);
			dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(GameLayer::onKeyReleased));
			dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(GameLayer::onKeyPressed));
			//dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(GameLayer::onMouseMoved));
			dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(GameLayer::onMouseClicked));
		}

	}

	inline static int tiles[2];
	static std::shared_ptr<Texture2D> m_Texture;
	static std::shared_ptr<OrthographicCameraController> m_CameraController;
	static std::shared_ptr<FrameBuffer> m_FrameBuffer;
	inline static bool onUI = false, isBox = true, m_LockCamera = false, m_Move = false;
	static char m_Text[128];
private:
	std::shared_ptr<Scene> m_ActiveScene;
	std::shared_ptr<Texture2D> m_newTexture;
	std::shared_ptr<Texture2D> m_TextureTest;
	std::vector<glm::vec2> positions;
	float m_X, m_Y;
	float m_LastFrameTime = 0.0f;
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
				glm::vec3 position = Math::ScreenToWorldPoint(glm::vec3(m_X, m_Y, 1.0f), m_CameraController->GetCamera().GetViewProjectionMatrix());

				if(isBox)
				{
					//Physics::CreateDynamicBox({ position.x,position.y,1.0f }, { 1.0f,1.0f });
					auto e = m_ActiveScene->CreateEntity();
					e.GetComponent<TransformComponent>().Translation = { position.x,position.y,0.0f };
					e.AddComponent<BoxCollider2DComponent>();
					e.AddComponent<RigidBody2DComponent>().BodyType = RigidBody2DComponent::Type::Dynamic;
					e.AddComponent<SpriteRendererComponent>().Color = { 1.0f,0.0f,0.0f,0.5 };
					auto& rb = e.GetComponent<RigidBody2DComponent>();


					return true;
				}
				else
				{
					auto e = m_ActiveScene->CreateEntity();
					e.GetComponent<TransformComponent>().Translation = { position.x,position.y,0.0f };
					e.AddComponent<CircleCollider2DComponent>();
					e.AddComponent<RigidBody2DComponent>().BodyType = RigidBody2DComponent::Type::Dynamic;
					e.AddComponent<SpriteRendererComponent>().Color = { 0.0f,1.0f,0.0f,0.5 };
					auto& rb = e.GetComponent<RigidBody2DComponent>();
					//Physics::CreateDynamicCircle({ position.x,position.y,1.0f }, 1.0f);
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
char GameLayer::m_Text[128];