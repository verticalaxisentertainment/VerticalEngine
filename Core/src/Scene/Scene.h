#pragma once

#include "Renderer/OrthographicCamera.h"
#include "UUID.h"
#include "entt.hpp"

#include "Core.h"

class Entity;

class _API Scene
{
public:
	Scene();
	~Scene();

	Entity CreateEntity(const std::string& name = std::string());
	Entity CreateEntityWithUUID(id::UUID uuid, const std::string& name = std::string());

	static std::shared_ptr<Scene> Copy(std::shared_ptr<Scene> other);
	entt::registry m_Registry;
private:
	void RenderScene(OrthographicCamera& camera);


	friend class Entity;
};