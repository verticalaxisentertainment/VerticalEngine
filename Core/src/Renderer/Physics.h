#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Core.h"

class _API Physics
{
public:
	static void CreateStaticBody(const glm::vec3& position, const glm::vec2& scale);
	static void CreateDynamicBox(const glm::vec3& position, const glm::vec2& scale);
	static void CreateDynamicCircle(const glm::vec3& position, const float& radius);

	static void Clear();

	static void Simulate(const float& timestep);
private:
	inline static float timeStep = 1.0f / 5000.0f;
	inline static int velocityIterations = 6;
	inline static int positionIterations = 2;
};