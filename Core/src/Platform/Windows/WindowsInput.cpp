#include "pch.h"
#include "Input.h"

#include "Application.h"
#include <GLFW/glfw3.h>

bool Input::IsKeyPressed(KeyCode key)
{
	auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetKey(window, static_cast<int32_t>(key));
	return state == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(MouseCode button)
{
	auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
	return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition()
{
	auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	return { (float)xPos,(float)yPos };
}

float Input::GetMouseX()
{
	return GetMousePosition().x;
}

float Input::GetMouseY()
{
	return GetMousePosition().y;
}

float Input::GetLocalMouseX()
{
	unsigned int width = Application::Get().GetWindow().GetWidth();
	if (GetMousePosition().x <= 0)
		return 0;
	if (GetMousePosition().x >= width)
		return width;
	return GetMousePosition().x;
}

float Input::GetLocalMouseY()
{
	unsigned int height = Application::Get().GetWindow().GetHeight();
	if (GetMousePosition().y <= 0)
		return 0;
	if (GetMousePosition().y >= height)
		return height;
	return GetMousePosition().y;
}





