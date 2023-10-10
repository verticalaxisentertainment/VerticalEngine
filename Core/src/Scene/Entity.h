#pragma once

#include "Components.h"
#include "Scene.h"

#include "entt.hpp"

class _API Entity
{
public:
	Entity() = default;
	Entity(entt::entity handle, Scene* scene);
	Entity(const Entity& other) = default;

	template<typename T,typename...Args>
	T& AddComponent(Args&&...args)
	{
		T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		return component;
	}

	template<typename T>
	T& GetComponent()
	{
		return m_Scene->m_Registry.get<T>(m_EntityHandle);
	}
	
	template<typename T>
	bool HasComponent()
	{
		return m_Scene->m_Registry.has<T>(m_EntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		m_Scene->m_Registry.remove<T>(m_EntityHandle);
	}

	id::UUID GetUUID() { return GetComponent<IDComponent>().ID; }

	~Entity();

private:
	entt::entity m_EntityHandle = entt::null;
	Scene* m_Scene = nullptr;
};
