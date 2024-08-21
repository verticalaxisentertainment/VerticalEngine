#include "pch.h"
#include "Scene.h"

#include "Renderer/Renderer.h"
#include "Renderer/UIRenderer.h"
#include "Entity.h"

#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>

#include "box2d/b2_circle_shape.h"
#include "box2d/b2_fixture.h"

#include "SceneSerializer.h"

#include "Renderer/OrthographicCameraController.h"
#include "Application.h"

Scene::Scene()
	:m_SceneName("Untitled")
{
	m_PhysicsWorld = new b2World({ 0.0f,-10.0f });

	Application& app = Application::Get();
	UIViewProjectionMatrix = glm::ortho(-static_cast<float>(app.GetWindow().GetWidth()) / 2, static_cast<float>(app.GetWindow().GetWidth()) / 2, -static_cast<float>(app.GetWindow().GetHeight()) / 2, static_cast<float>(app.GetWindow().GetHeight()) / 2);

	//UIViewProjectionMatrix = glm::ortho(-1920.0f / 2, 1080.0f / 2, -1080.0f / 2, 1920.0f / 2);
}

Scene::Scene(const std::string& name)
	:m_SceneName(name)
{
	m_PhysicsWorld = new b2World({ 0.0f,-10.0f });

	Application& app = Application::Get();
	UIViewProjectionMatrix = glm::ortho(-static_cast<float>(app.GetWindow().GetWidth()) / 2, static_cast<float>(app.GetWindow().GetWidth()) / 2, -static_cast<float>(app.GetWindow().GetHeight()) / 2, static_cast<float>(app.GetWindow().GetHeight()) / 2);
	//UIViewProjectionMatrix = glm::ortho(-1920.0f / 2, 1080.0f / 2, -1080.0f / 2, 1920.0f / 2);

	m_Width = (float)app.GetWindow().GetWidth();
	m_Height = (float)app.GetWindow().GetHeight();
}

Scene::~Scene()
{
}

Entity Scene::CreateEntity(const std::string& name)
{
	return CreateEntityWithUUID(id::UUID(), name);
}

Entity Scene::CreateEntityWithUUID(id::UUID uuid, const std::string& name)
{
	Entity entity = { m_Registry.create(),this };
	entity.AddComponent<IDComponent>(uuid);
	entity.AddComponent<TransformComponent>();

	return entity;
}

std::shared_ptr<Scene> Scene::Copy(std::shared_ptr<Scene> other)
{
	return nullptr;
}

void Scene::RenderScene(OrthographicCamera& camera)
{
	Renderer::BeginScene(camera);
	auto view = m_Registry.view<TransformComponent>();
	for (auto e : view)
	{
		Entity entity = { e,this };
		if (!entity.HasComponent<UITransformComponent>())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			if (entity.HasComponent<SpriteRendererComponent>())
			{
				auto& sprite = entity.GetComponent<SpriteRendererComponent>();
				if (!sprite.Texture)
					Renderer::DrawQuad(transform.GetTranform(), sprite.Color);
				else
					Renderer::DrawQuad(transform.GetTranform(), sprite.Texture);
			}
			if (entity.HasComponent<CircleRendererComponent>())
			{
				auto& circle = entity.GetComponent<CircleRendererComponent>();
				Renderer::DrawCircle(transform.GetTranform(), circle.Color, circle.Thickness, circle.Fade);
			}
			if (entity.HasComponent<LineRendererComponent>())
			{
				auto& line = entity.GetComponent<LineRendererComponent>();
				Renderer::DrawLine(line.Point1Position, line.Point2Position, line.Color);
			}
			if (entity.HasComponent<TextRendererComponent>())
			{
				auto& text = entity.GetComponent<TextRendererComponent>();
				Renderer::RenderText(text.Text, transform.Translation, text.Scale, text.Color);
			}
		}
	}

	Renderer::EndScene();

	Renderer::BeginScene();
	for (auto e : view)
	{
		Entity entity = { e,this };

		if (entity.HasComponent<UITransformComponent>())
		{
			auto& transform = entity.GetComponent<UITransformComponent>();

			if (entity.HasComponent<SpriteRendererComponent>())
			{
				auto& sprite = entity.GetComponent<SpriteRendererComponent>();
				Renderer::DrawQuad(transform.Translation, transform.Scale, sprite.Color, entity.GetUUID().id());
			}
			if (entity.HasComponent<CircleRendererComponent>())
			{
				auto& circle = entity.GetComponent<CircleRendererComponent>();
				Renderer::DrawCircle(transform.GetTranform(), circle.Color, circle.Thickness, circle.Fade, entity.GetUUID().id());
			}
			if (entity.HasComponent<TextRendererComponent>())
			{
				auto& text = entity.GetComponent<TextRendererComponent>();
				Renderer::RenderText(text.Text, transform.Translation, text.Scale, text.Color);
			}
		}
	}

	Renderer::EndScene();
}

Entity& Scene::GetEntityWithUUID(const uint32_t& id)
{
	auto view = m_Registry.view<IDComponent>();
	for (auto e : view)
	{
		Entity entity = { e,this };
		if (entity.GetComponent<IDComponent>().ID.id() == id)
			return entity;
	}
}

void Scene::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Scene::OnWindowResize));
}

bool Scene::OnWindowResize(WindowResizeEvent& e)
{
	float ratio = (float)e.GetWidth() / (float)e.GetHeight();

	auto view = m_Registry.view<UITransformComponent>();
	for (auto en : view)
	{
		Entity entity = { en,this };
		auto& transform = entity.GetComponent<UITransformComponent>();
		transform.Translation.x += (((float)e.GetWidth() - m_Width) / 2);
		transform.Translation.y += (((float)e.GetHeight() - m_Height) / 2);
	}

	m_Width = e.GetWidth();
	m_Height = e.GetHeight();

	return false;
}

void Scene::OnPhysics2DStart()
{
	auto view = m_Registry.view<RigidBody2DComponent>();
	for (auto e : view)
	{
		Entity entity(e, this);

		auto& rigidbody = entity.GetComponent<RigidBody2DComponent>();
		auto& transform = entity.GetComponent<TransformComponent>();

		b2BodyDef bodyDef;
		bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
		switch (rigidbody.BodyType)
		{
			case RigidBody2DComponent::Type::Dynamic: bodyDef.type = b2BodyType::b2_dynamicBody; break;
			case RigidBody2DComponent::Type::Static: bodyDef.type = b2BodyType::b2_staticBody; break;
		}

		rigidbody.Body = m_PhysicsWorld->CreateBody(&bodyDef);

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& collider = entity.GetComponent<BoxCollider2DComponent>();
			b2PolygonShape boxShape;
			boxShape.SetAsBox(transform.Scale.x / 2, transform.Scale.y / 2);
			
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.density = collider.Density;
			fixtureDef.friction = collider.Friction;
			static_cast<b2Body*>(rigidbody.Body)->CreateFixture(&fixtureDef);
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			auto& collider = entity.GetComponent<CircleCollider2DComponent>();

			b2CircleShape circleShape;
			circleShape.m_p.SetZero();
			circleShape.m_radius = collider.Radius;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circleShape;
			fixtureDef.density = collider.Density;
			fixtureDef.friction = collider.Friction;
			static_cast<b2Body*>(rigidbody.Body)->CreateFixture(&fixtureDef);
		}
	}
}

void Scene::Simulate(const float& timestep)
{
	m_PhysicsWorld->Step(timestep, velocityIterations, positionIterations);

	auto view = m_Registry.view<RigidBody2DComponent>();
	for (auto e : view)
	{
		Entity entity(e, this);
		auto& rigidbody= entity.GetComponent<RigidBody2DComponent>();
		auto& transform = entity.GetComponent<TransformComponent>();

		auto position = static_cast<b2Body*>(rigidbody.Body)->GetPosition();
		auto rotation = static_cast<b2Body*>(rigidbody.Body)->GetAngle();

		transform.Translation.x = position.x;
		transform.Translation.y = position.y;
		transform.Rotation.z = rotation;
	}

}

template<>
void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
{
	auto& transform = entity.GetComponent<TransformComponent>();

	b2BodyDef bodyDef;
	bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
	switch (component.BodyType)
	{
		case RigidBody2DComponent::Type::Dynamic: bodyDef.type = b2BodyType::b2_dynamicBody; break;
		case RigidBody2DComponent::Type::Static: bodyDef.type = b2BodyType::b2_staticBody; break;
	}

	component.Body = m_PhysicsWorld->CreateBody(&bodyDef);

	if (entity.HasComponent<BoxCollider2DComponent>())
	{
		auto& collider = entity.GetComponent<BoxCollider2DComponent>();
		b2PolygonShape boxShape;
		boxShape.SetAsBox(transform.Scale.x / 2, transform.Scale.y / 2);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &boxShape;
		fixtureDef.density = collider.Density;
		fixtureDef.friction = collider.Friction;
		static_cast<b2Body*>(component.Body)->CreateFixture(&fixtureDef);
	}

	if (entity.HasComponent<CircleCollider2DComponent>())
	{
		auto& collider = entity.GetComponent<CircleCollider2DComponent>();

		b2CircleShape circleShape;
		circleShape.m_p.SetZero();
		circleShape.m_radius = collider.Radius;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circleShape;
		fixtureDef.density = collider.Density;
		fixtureDef.friction = collider.Friction;
		static_cast<b2Body*>(component.Body)->CreateFixture(&fixtureDef);
	}

	auto bodylist=m_PhysicsWorld->GetBodyList();
}

template<>
void Scene::OnComponentChanged(Entity entity, RigidBody2DComponent& component)
{
	switch (component.BodyType)
	{
		case RigidBody2DComponent::Type::Dynamic: static_cast<b2Body*>(component.Body)->SetType(b2BodyType::b2_dynamicBody); break;
		case RigidBody2DComponent::Type::Static: static_cast<b2Body*>(component.Body)->SetType(b2BodyType::b2_staticBody); break;
	}
}
