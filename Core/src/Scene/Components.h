#pragma once

#include "UUID.h"
#include "Renderer/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


struct IDComponent
{
	id::UUID ID;

	IDComponent() = default;
	IDComponent(const IDComponent&) = default;
};

struct TransformComponent
{
	glm::vec3 Translation = { 0.0f,0.0f,0.0f };
	glm::vec3 Rotation = { 0.0f,0.0f,0.0f };
	glm::vec3 Scale = { 1.0f,1.0f,1.0f };

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::vec3& translation)
		:Translation(translation) {}

	glm::mat4 GetTranform() const
	{
		glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

		return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
	}
};

struct SpriteRendererComponent
{
	glm::vec4 Color = { 0.0f,0.0f,0.0f,1.0f };
	std::shared_ptr<Texture2D> Texture;

	SpriteRendererComponent() = default;
	SpriteRendererComponent(const SpriteRendererComponent&) = default;
	SpriteRendererComponent(const glm::vec4& color) :Color(color) {}
};

struct CircleRendererComponent
{
	glm::vec4 Color = { 0.0f,0.0f,0.0f,1.0f };
	float Thickness = 1.0f;
	float Fade = 0.005f;

	CircleRendererComponent() = default;
	CircleRendererComponent(const CircleRendererComponent&) = default;
	CircleRendererComponent(const glm::vec4& color) :Color(color) {}
};

struct RigidBody2DComponent
{
	enum class Type { Static = 0, Dynamic };
	Type BodyType = Type::Static;

	void* Body = nullptr;

	RigidBody2DComponent() = default;
	RigidBody2DComponent(const RigidBody2DComponent&) = default;
};

struct BoxCollider2DComponent
{
	glm::vec2 Offset = { 0.0f,0.0f };
	glm::vec2 Size = { 0.5f,0.5f };

	float Density = 1.0f;
	float Friction = 0.3f;

	BoxCollider2DComponent() = default;
	BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
};
struct CircleCollider2DComponent
{
	glm::vec2 Offset = { 0.0f,0.0f };
	float Radius = 0.5f;

	float Density = 1.0f;
	float Friction = 0.5f;

	CircleCollider2DComponent() = default;
	CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
};
