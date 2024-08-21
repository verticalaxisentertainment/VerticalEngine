#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Core.h"
#include "UUID.h"

#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

class _API UIRenderer
{
public:
	static void Init();
	static void StartBatch();
	static void Flush();

	static void DrawUIQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int id = 1);
	static void DrawUIQuad(const glm::mat4& transform,const glm::vec4& color, int id = 1);

	static void DrawUICircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int id = 2);

	static void DrawProgressBar(const float& progress);

	static int GetPickedID();

	static void OnEvent(Event& e);

	static bool IsHovered(const int& ID);
	static bool IsClicked(const int& ID);

	inline static bool m_onUI;
	inline static glm::vec2 m_ScaleFactor;
private:
	inline static int m_Width;
	inline static int m_Height;
	static bool OnMouseClicked(MouseButtonPressedEvent& e);
	static bool OnMouseReleased(MouseButtonReleasedEvent& e);
	static bool OnWindowResize(WindowResizeEvent& e);
	inline static bool m_MouseClicked;
};