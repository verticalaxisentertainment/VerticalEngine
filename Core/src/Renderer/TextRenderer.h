#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "VertexArray.h"

struct TextData
{
	std::vector<glm::vec4> Vertices;
	unsigned int TextureID;
};

class TextRenderer
{
public:
	static void Init();

	static TextData RenderText(const std::string& text, glm::vec2 position, const float& scale, const glm::vec4& color, std::shared_ptr<VertexArray> vertexArray, const glm::mat4& projection);

private:
};
