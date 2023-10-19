#include "pch.h"
#include "Scene.h"

#include "Renderer/Renderer.h"
#include "Entity.h"

#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>

#include "box2d/b2_circle_shape.h"
#include "box2d/b2_fixture.h"


Scene::Scene()
{
	m_PhysicsWorld = new b2World({ 0.0f,-10.0f });
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
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& sprite = entity.GetComponent<SpriteRendererComponent>();
			Renderer::DrawQuad(transform.GetTranform(), sprite.Color);
		}
		
		if (entity.HasComponent<CircleRendererComponent>())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& circle = entity.GetComponent<CircleRendererComponent>();
			Renderer::DrawCircle(transform.GetTranform(), circle.Color, circle.Thickness, circle.Fade);
		}
	}

	Renderer::EndScene();
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

template<typename T>
void Scene::OnComponentAdded(Entity entity, T& component)
{

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
void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
{

}

template<>
void Scene::OnComponentAdded(Entity entity, SpriteRendererComponent& component)
{
}

template<>
void Scene::OnComponentAdded(Entity entity, CircleCollider2DComponent& component)
{
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
