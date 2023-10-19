#pragma once

#include "Renderer/OrthographicCamera.h"
#include "UUID.h"
#include "entt.hpp"

#include "Core.h"
#include "Components.h"

class Entity;
class b2World;

class _API Scene
{
public:
	Scene();
	~Scene();

	Entity CreateEntity(const std::string& name = std::string());
	Entity CreateEntityWithUUID(id::UUID uuid, const std::string& name = std::string());

	void OnPhysics2DStart();
	void Simulate(const float& timestep);

	static std::shared_ptr<Scene> Copy(std::shared_ptr<Scene> other);
	entt::registry m_Registry;
	void RenderScene(OrthographicCamera& camera);

private:
	b2World* m_PhysicsWorld = nullptr;
	inline static int velocityIterations = 6;
	inline static int positionIterations = 2;

	friend class Entity;
private:
	template<typename T> void OnComponentAdded(Entity entity, T& component);
	template<> void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component);
	template<> void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component);
	template<> void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component);
	template<> void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component);
private:
	template<typename T>
	void OnComponentChanged(Entity entity, T& component)
	{
		m_Registry.patch<T>(entity);
	}

	template<> void Scene::OnComponentChanged<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component);
};