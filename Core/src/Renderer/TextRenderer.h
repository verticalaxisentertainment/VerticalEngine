#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "VertexArray.h"
#include "Texture.h"


struct TextData
{
	std::vector<glm::vec4> Vertices;
	std::vector<std::shared_ptr<Texture>> Texture;
};

class TextRenderer
{
public:
	static void Init();

	static TextData RenderText(const std::string& text, glm::vec2 position, const float& scale);

private:
};
