#pragma once

#include "KeyCode.h"
#include "MouseCode.h"

#include <glm/glm.hpp>
#include "Core.h"

class _API Input
{
public:
	static bool IsKeyPressed(KeyCode key);

	static bool IsMouseButtonPressed(MouseCode button);
	static glm::vec2 GetMousePosition();
	static float GetMouseX();
	static float GetMouseY();
	static float GetLocalMouseX();
	static float GetLocalMouseY();
};
