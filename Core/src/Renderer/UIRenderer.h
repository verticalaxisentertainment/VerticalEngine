#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Core.h"
#include "UUID.h"


class _API UI
{
public:
	static void SetPixelData(float* pixelData);
	static float* GetPixelData();
};

class _API UIQuad
{
public:
	UIQuad();
	~UIQuad();

	void Render(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
	bool IsClicked();
	bool IsHovered();

private:
	unsigned int m_ID;

	glm::vec3 m_Position;
	glm::vec2 m_Scale;
	glm::vec4 m_Color;
};



class _API UICircle
{
public:
	UICircle();
	~UICircle();

	void Render(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
	bool IsClicked();
	bool IsHovered();

private:
	unsigned int m_ID;

	glm::vec3 m_Position;
	glm::vec2 m_Scale;
	glm::vec4 m_Color;
};

