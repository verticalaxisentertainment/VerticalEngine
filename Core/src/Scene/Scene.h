#pragma once

#include "Renderer/OrthographicCamera.h"
#include "UUID.h"
#include "entt.hpp"

#include "Core.h"
#include "Components.h"

class Entity;
class b2World;
class Event;
class WindowResizeEvent;

class _API Scene
{
public:
	Scene();
	Scene(const std::string& name);
	~Scene();

	Entity CreateEntity(const std::string& name = std::string());
	Entity CreateEntityWithUUID(id::UUID uuid, const std::string& name = std::string());

	void OnPhysics2DStart();
	void Simulate(const float& timestep);

	static std::shared_ptr<Scene> Copy(std::shared_ptr<Scene> other);
	entt::registry m_Registry;
	void RenderScene(OrthographicCamera& camera);
	Entity& GetEntityWithUUID(const uint32_t& id);

	std::string m_SceneName;
	glm::mat4 UIViewProjectionMatrix;

	void OnEvent(Event& e);
private:
	b2World* m_PhysicsWorld = nullptr;
	inline static int velocityIterations = 6;
	inline static int positionIterations = 2;

	inline static int m_Width;
	inline static int m_Height;
	friend class Entity;
private:
	bool OnWindowResize(WindowResizeEvent& e);
private:
	template<typename T> void OnComponentAdded(Entity entity, T& component){}
	template<> void OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component);
private:
	template<typename T> void OnComponentChanged(Entity entity, T& component) { /*m_Registry.patch<T>(entity);*/ }
	template<> void Scene::OnComponentChanged<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component);
};