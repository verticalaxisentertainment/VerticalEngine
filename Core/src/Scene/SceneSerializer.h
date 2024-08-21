#pragma once
#include "Core.h"

class Entity;
class Scene;

class _API SceneSerializer
{
public:
	static void Create(const std::string& name);

	static void InitEntity(Entity& entity,const std::string& name);
	static void Read(const std::string& sceneFile, const std::shared_ptr<Scene>& scene);
};