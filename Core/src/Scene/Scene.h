#pragma once

#include "Renderer/OrthographicCamera.h"

class Scene
{
public:
	Scene();
	~Scene();

	static std::shared_ptr<Scene> Copy(std::shared_ptr<Scene> other);
private:
	void RenderScene(OrthographicCamera& camera);
};