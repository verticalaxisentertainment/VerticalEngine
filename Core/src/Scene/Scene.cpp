#include "pch.h"
#include "Scene.h"

#include "Renderer/Renderer.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

std::shared_ptr<Scene> Scene::Copy(std::shared_ptr<Scene> other)
{
	return nullptr;
}

void Scene::RenderScene(OrthographicCamera& camera)
{
	Renderer::BeginScene(camera);
}
