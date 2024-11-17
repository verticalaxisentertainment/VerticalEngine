#include "pch.h"
#include "ComponentEvent.h"
#include "Scene/Components.h"

#include "Scene/Entity.h"

#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>

#include "box2d/b2_circle_shape.h"
#include "box2d/b2_fixture.h"

// Define the template functions for each component type
template <>
void ComponentEvent<int>::OnComponentChanged(Entity entity, int &component)
{
	ERROR(component);
}

// Define the template functions for each component type
template <>
void ComponentEvent<int>::OnComponentAdded(Entity entity, int &component)
{
	ERROR(component);
}

// Define the template functions for each component type
template <>
void ComponentEvent<IDComponent>::OnComponentChanged(Entity entity, IDComponent &component)
{
	// Specialization code for IDComponent
}

template <>
void ComponentEvent<IDComponent>::OnComponentAdded(Entity entity, IDComponent &component)
{
	// Specialization code for IDComponent
}

template <>
void ComponentEvent<TransformComponent>::OnComponentChanged(Entity entity, TransformComponent &component)
{
	// Specialization code for TransformComponent
}

template <>
void ComponentEvent<TransformComponent>::OnComponentAdded(Entity entity, TransformComponent &component)
{
	// Specialization code for TransformComponent
}

template <>
void ComponentEvent<UITransformComponent>::OnComponentChanged(Entity entity, UITransformComponent &component)
{
	// Specialization code for UITransformComponent
}

template <>
void ComponentEvent<UITransformComponent>::OnComponentAdded(Entity entity, UITransformComponent &component)
{
	// Specialization code for UITransformComponent
}

template <>
void ComponentEvent<SpriteRendererComponent>::OnComponentChanged(Entity entity, SpriteRendererComponent &component)
{
	// Specialization code for SpriteRendererComponent
}

template <>
void ComponentEvent<SpriteRendererComponent>::OnComponentAdded(Entity entity, SpriteRendererComponent &component)
{
	// Specialization code for SpriteRendererComponent
}

template <>
void ComponentEvent<CircleRendererComponent>::OnComponentChanged(Entity entity, CircleRendererComponent &component)
{
	// Specialization code for CircleRendererComponent
}

template <>
void ComponentEvent<CircleRendererComponent>::OnComponentAdded(Entity entity, CircleRendererComponent &component)
{
	// Specialization code for CircleRendererComponent
}

template <>
void ComponentEvent<LineRendererComponent>::OnComponentChanged(Entity entity, LineRendererComponent &component)
{
	// Specialization code for LineRendererComponent
}

template <>
void ComponentEvent<LineRendererComponent>::OnComponentAdded(Entity entity, LineRendererComponent &component)
{
	// Specialization code for LineRendererComponent
}

template <>
void ComponentEvent<RigidBody2DComponent>::OnComponentChanged(Entity entity, RigidBody2DComponent &component)
{
	switch (component.BodyType)
	{
	case RigidBody2DComponent::Type::Dynamic:
		static_cast<b2Body *>(component.Body)->SetType(b2BodyType::b2_dynamicBody);
		break;
	case RigidBody2DComponent::Type::Static:
		static_cast<b2Body *>(component.Body)->SetType(b2BodyType::b2_staticBody);
		break;
	}
}

template <>
void ComponentEvent<RigidBody2DComponent>::OnComponentAdded(Entity entity, RigidBody2DComponent &component)
{
	auto &transform = entity.GetComponent<TransformComponent>();

	b2BodyDef bodyDef;
	bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
	switch (component.BodyType)
	{
	case RigidBody2DComponent::Type::Dynamic:
		bodyDef.type = b2BodyType::b2_dynamicBody;
		break;
	case RigidBody2DComponent::Type::Static:
		bodyDef.type = b2BodyType::b2_staticBody;
		break;
	}

	component.Body = entity.GetScene()->GetPhysicsWorld()->CreateBody(&bodyDef);

	if (entity.HasComponent<BoxCollider2DComponent>())
	{
		auto &collider = entity.GetComponent<BoxCollider2DComponent>();
		b2PolygonShape boxShape;
		boxShape.SetAsBox(transform.Scale.x / 2, transform.Scale.y / 2);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &boxShape;
		fixtureDef.density = collider.Density;
		fixtureDef.friction = collider.Friction;
		static_cast<b2Body *>(component.Body)->CreateFixture(&fixtureDef);
	}

	if (entity.HasComponent<CircleCollider2DComponent>())
	{
		auto &collider = entity.GetComponent<CircleCollider2DComponent>();

		b2CircleShape circleShape;
		circleShape.m_p.SetZero();
		circleShape.m_radius = collider.Radius;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circleShape;
		fixtureDef.density = collider.Density;
		fixtureDef.friction = collider.Friction;
		static_cast<b2Body *>(component.Body)->CreateFixture(&fixtureDef);
	}

	auto bodylist = entity.GetScene()->GetPhysicsWorld()->GetBodyList();
}

template <>
void ComponentEvent<BoxCollider2DComponent>::OnComponentChanged(Entity entity, BoxCollider2DComponent &component)
{
	// Specialization code for BoxCollider2DComponent
}

template <>
void ComponentEvent<BoxCollider2DComponent>::OnComponentAdded(Entity entity, BoxCollider2DComponent &component)
{
	// Specialization code for BoxCollider2DComponent
}

template <>
void ComponentEvent<CircleCollider2DComponent>::OnComponentChanged(Entity entity, CircleCollider2DComponent &component)
{
	// Specialization code for CircleCollider2DComponent
}

template <>
void ComponentEvent<CircleCollider2DComponent>::OnComponentAdded(Entity entity, CircleCollider2DComponent &component)
{
	// Specialization code for CircleCollider2DComponent
}

template <>
void ComponentEvent<TextRendererComponent>::OnComponentChanged(Entity entity, TextRendererComponent &component)
{
	// Specialization code for TextRendererComponent
}

template <>
void ComponentEvent<TextRendererComponent>::OnComponentAdded(Entity entity, TextRendererComponent &component)
{
	// Specialization code for TextRendererComponent
}
