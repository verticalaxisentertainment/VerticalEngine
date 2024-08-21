#pragma once
#include "DLL.h"

#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

class GameLayer :public Layer
{
public:
	GameLayer()
		:Layer("GameLayer")
	{
		Instance = this;
		m_CameraController.reset(new OrthographicCameraController(1280.0f / 720.0f, true));
		m_ActiveScene.reset(new Scene("Sandbox"));
	}

	void OnAttach() override
	{
		auto& window = Application::Get().GetWindow();

		FrameBufferSpecification fbSpec;
		fbSpec.Width = window.GetWidth();
		fbSpec.Height = window.GetHeight();
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

		textEntity = m_ActiveScene->CreateEntity();
		textEntity.GetComponent<TransformComponent>().Translation = { -5.0f,2.0f,-0.8f };
		auto& text = textEntity.AddComponent<TextRendererComponent>();
		text.Color = { 0.0f,1.0f,0.5f,1.0f };
		text.Text = m_Text;

		entity = m_ActiveScene->CreateEntity();
		entity.GetComponent<TransformComponent>().Translation = { 0.0f, 5.0f, -0.1f };
		entity.AddComponent<SpriteRendererComponent>().Texture.reset(Texture2D::Create(Application::Get().m_IMGUIFrameBuffer.get()));

		entity = m_ActiveScene->CreateEntity();
		entity.GetComponent<TransformComponent>().Translation = { 1.0f, 5.0f, 0.0f };
		entity.AddComponent<SpriteRendererComponent>().Texture.reset(Texture2D::Create("assets/textures/red.jpg"));

		quadUI = m_ActiveScene->CreateEntity();
		auto& ui = quadUI.AddComponent<UITransformComponent>();
		ui.Translation = { window.GetWidth() / 2 - 180.0f ,window.GetHeight() / 2 - 80.0f ,1.0f };
		ui.Scale = { 100 ,100 ,1 };
		quadUI.AddComponent<SpriteRendererComponent>().Color = { 1.0,0.0f,0.0f,1.0f };

		circleUI = m_ActiveScene->CreateEntity();
		auto& circleui = circleUI.AddComponent<UITransformComponent>();
		circleui.Translation = { window.GetWidth() / 2 - 60.0f ,window.GetHeight() / 2 - 80.0f ,1.0f };
		circleui.Scale = { 100 ,100 ,1 };
		circleUI.AddComponent<CircleRendererComponent>().Color = { 0.0,1.0f,0.0f,1.0f };

		backgroundEntity = m_ActiveScene->CreateEntity();
		auto& backgroundUI = backgroundEntity.AddComponent<UITransformComponent>();
		backgroundUI.Scale = { 250.0f  ,150.0f ,1.0f };
		backgroundUI.Translation = { window.GetWidth() / 2 - (backgroundUI.Scale.x / 2) ,window.GetHeight() / 2 - (backgroundUI.Scale.y / 2) ,0.6f };
		backgroundEntity.AddComponent<SpriteRendererComponent>().Color = { 0.38f,0.38f,0.38f,0.5f };

		entity = m_ActiveScene->CreateEntity();
		entity.AddComponent<UITransformComponent>().Translation = { window.GetWidth() / 2 - 240.0f,window.GetHeight() / 2 - 180.0f,1.0f };
		auto& textUI = entity.AddComponent<TextRendererComponent>();
		textUI.Text = "Select a Shape";
		textUI.Scale = 0.7f;

		pickingShape = m_ActiveScene->CreateEntity();
		pickingShape.AddComponent<SpriteRendererComponent>().Color = { 1.0f, 1.0f, 1.0f, 0.5f };

		/*debugShape = m_ActiveScene->CreateEntity();
		auto& tr = debugShape.AddComponent<UITransformComponent>();
		tr.Translation = {ui.Translation.x,ui.Translation.y,1.0f };
		tr.Scale = { 50.0f,50.0f,1.0f };
		debugShape.AddComponent<CircleRendererComponent>().Color = { 1.0f,1.0f,0.0f,1.0f };
		*/
		
		SceneSerializer::Create(m_ActiveScene->m_SceneName);
		auto view = m_ActiveScene->m_Registry.view<TransformComponent>();
		for (auto e : view)
		{
			Entity entity = { e,m_ActiveScene.get() };
			SceneSerializer::InitEntity(entity, m_ActiveScene->m_SceneName);
		}

		//m_ActiveScene->OnPhysics2DStart();
		//Application::Get().texture.reset((Texture2D::Create(m_FrameBuffer.get())));
	}

	

	void OnUpdate() override
	{
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Application& app = Application::Get();
		m_X = Input::GetLocalMouseX();
		m_Y = Input::GetLocalMouseY();

		float time = Math::Time();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;
		auto& pos = m_CameraController->GetCamera().GetPosition();
		//RenderCommand::Clear();
		if (!m_LockCamera && m_Move)
		{
			m_CameraController->SetPosition(pos);
			m_CameraController->OnUpdate(timestep);
		}

		//OrthographicCameraController cam(1280.0f / 720.0f, true);

		glm::vec3 mouseWorldPos = Math::ScreenToWorldPoint(glm::vec3(m_X, m_Y, 1.0f), m_CameraController->GetCamera().GetViewProjectionMatrix());

		pickingShape.GetComponent<TransformComponent>().Translation = { mouseWorldPos.x,mouseWorldPos.y,0.5f };
		

		textEntity.GetComponent<TextRendererComponent>().Text = m_Text;
		m_FrameBuffer->Bind();
		RenderCommand::Clear();
		m_ActiveScene->RenderScene(m_CameraController->GetCamera());
		app.GetFrameBuffer() = m_FrameBuffer;

		if (circleUI.IsHovered() || quadUI.IsHovered())
		{
			app.GetWindow().SetCursor(Cursor::HAND);
			onUI = true;
		}
		else
		{
			app.GetWindow().SetCursor(Cursor::ARROW);
			onUI = false;
		}

		if (circleUI.IsClicked())
		{
			circleUI.GetComponent<CircleRendererComponent>().Color = { 0.0f,0.0f,0.0f,1.0f };
			if (pickingShape.HasComponent<SpriteRendererComponent>() && !pickingShape.HasComponent<CircleRendererComponent>())
			{
				pickingShape.RemoveComponent<SpriteRendererComponent>();
				pickingShape.AddComponent<CircleRendererComponent>().Color = { 1.0f,1.0f,1.0f,0.5f };
				isBox = false;
			}
		}
		else if (quadUI.IsClicked())
		{
			quadUI.GetComponent<SpriteRendererComponent>().Color = { 0.0f,0.0f,0.0f,1.0f };
			if (pickingShape.HasComponent<CircleRendererComponent>() && !pickingShape.HasComponent<SpriteRendererComponent>())
			{
				pickingShape.RemoveComponent<CircleRendererComponent>();
				pickingShape.AddComponent<SpriteRendererComponent>().Color = { 1.0f,1.0f,1.0f,0.5f };
				isBox = true;
			}

		}
		else
		{
			quadUI.GetComponent<SpriteRendererComponent>().Color = { 1.0f,0.0f,0.0f,1.0f };
			circleUI.GetComponent<CircleRendererComponent>().Color = { 0.0f,1.0f,0.0f,1.0f };
		}
		m_FrameBuffer->UnBind();
		RenderCommand::Clear();
		Renderer::DrawFrameBuffer(m_FrameBuffer);
		
		m_FrameBuffer->Bind();
		RenderCommand::Clear();
		Renderer::BeginScene(m_CameraController->GetCamera());
		Renderer::DrawQuad({ 0.0f,0.0f,1.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		Renderer::RenderText("Deneme123", { 0.0f,-2.0f,1.0f }, 0.01f, { 1.0f,1.0f,1.0f,1.0f });
		Renderer::EndScene();
		m_FrameBuffer->UnBind();

		/*Renderer::DrawLine({ glm::sin(Math::Time()) / 2,-glm::cos(Math::Time()) / 2,1.0f }, { 0.0f,0.0f,1.0f }, { 0.0f,0.0f,1.0f,1.0f });
		Renderer::DrawLine({ -0.5f,0.0f,1.0f }, { 0.5f,0.0f,1.0f }, { 1.0f,0.0,0.0f,1.0f });
		Renderer::DrawLine({ 0.0f,-0.5f,1.0f }, { 0.0f,0.5f,1.0f }, { 0.0f,1.0,0.0f,1.0f });*/
		m_ActiveScene->Simulate(timestep);

		Renderer::BeginScene();
		Renderer::DrawQuad({ 100.0f,0.0f,1.0f }, { 100.0f*progress,100.0f }, { 0.0f,1.0f,0.0f,1.0f });
		Renderer::EndScene();
	}

	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
		if (!e.Handled)
		{
			m_Move = true;
			m_CameraController->OnEvent(e);
			UIRenderer::OnEvent(e);
			m_ActiveScene->OnEvent(e);
			//m_CameraController->CanMove(true);
			dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(GameLayer::onKeyReleased));
			dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(GameLayer::onKeyPressed));
			//dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(GameLayer::onMouseMoved));
			dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(GameLayer::onMouseClicked));
			
		}

	}


public:
	int tiles[2];
	std::shared_ptr<OrthographicCameraController> m_CameraController;
	bool isBox = true, m_LockCamera = false, m_Move = false;
	char m_Text[128]="Lorem Ipsulum";
	std::shared_ptr<Scene> m_ActiveScene;
	std::shared_ptr<FrameBuffer> m_FrameBuffer;
	float progress = 0.0f;

	static GameLayer* Instance;
private:
	float m_Width = 1200.0f, m_Height = 720.0f;
	glm::vec2 m_ScaleFactor;
	Entity quadUI, circleUI, textEntity, backgroundEntity, pickingShape,debugShape;
	float m_X, m_Y;
	float m_LastFrameTime = 0.0f;
	bool onUI = false;
	

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
		m_X = e.GetX();
		m_Y = e.GetY();
		return true;
	}

	bool onMouseClicked(MouseButtonPressedEvent& e)
	{
		if(!e.Handled)
		{
			if (e.GetMouseButton() == Mouse::Button0 && !onUI)
			{
				glm::vec3 position = Math::ScreenToWorldPoint(glm::vec3(m_X, m_Y, 1.0f), m_CameraController->GetCamera().GetViewProjectionMatrix());

				if(isBox)
				{
					auto e = m_ActiveScene->CreateEntity();
					e.GetComponent<TransformComponent>().Translation = { position.x,position.y,-0.1f };
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
					e.AddComponent<CircleRendererComponent>().Color = { 0.0f,1.0f,0.0f,0.5 };
					auto& rb = e.GetComponent<RigidBody2DComponent>();

					return true;
				}
			}
		}
		return false;
	}
};

GameLayer* GameLayer::Instance = nullptr;