#include "pch.h"
#include "Scene.h"

#include "Renderer/Renderer.h"
#include "Entity.h"

Scene::Scene()
{
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
}
