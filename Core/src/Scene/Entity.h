#pragma once

#include "Events/ComponentEvent.h"
#include "Components.h"
#include "Scene.h"

#include "entt.hpp"

class _API Entity
{
public:
	Entity() = default;
	Entity(entt::entity handle, Scene *scene);
	Entity(const Entity &other) = default;

	template <typename T, typename... Args>
	T &AddComponent(Args &&...args)
	{
		T &component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		int a = 5;
		ComponentEvent<int>::OnComponentAdded(*this, a);
		return component;
	}

	template <typename T>
	T &GetComponent()
	{
		T &component = m_Scene->m_Registry.get<T>(m_EntityHandle);
		int a = 5;
		ComponentEvent<int>::OnComponentChanged(*this, a);
		return component;
	}

	template <typename T>
	bool HasComponent()
	{
		return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
	}

	template <typename T>
	void RemoveComponent()
	{
		m_Scene->m_Registry.remove<T>(m_EntityHandle);
	}

	bool IsHovered();
	bool IsClicked();

	inline Scene *GetScene() const { return m_Scene; }

	id::UUID GetUUID() { return GetComponent<IDComponent>().ID; }

	operator bool() const { return m_EntityHandle != entt::null; }
	operator entt::entity() const { return m_EntityHandle; }
	operator uint32_t() const { return (uint32_t)m_EntityHandle; }

	bool operator==(const Entity &other) const
	{
		return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
	}

	bool operator!=(const Entity &other) const
	{
		return !(*this == other);
	}

private:
	entt::entity m_EntityHandle{entt::null};
	Scene *m_Scene = nullptr;
};
