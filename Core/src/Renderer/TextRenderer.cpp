#include "pch.h"
#include "TextRenderer.h"
#include "Buffer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>

struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};
std::map<GLchar, Character> Characters;



void TextRenderer::Init()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// FreeType
	// --------
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, "assets/fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}


	// set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// load first 128 characters of ASCII set
	for (unsigned char c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		unsigned int texture;
		// generate texture
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x)
		};
		Characters.insert(std::pair<char, Character>(c, character));

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

TextData TextRenderer::RenderText(const std::string& text, glm::vec2 position, const float& scale, const glm::vec4& color,std::shared_ptr<VertexArray> vertexArray,const glm::mat4& projection)
{
	TextData textData;
	uint32_t indices[] = { 0,1,2,0,2,3 };

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];
		float xpos = position.x + ch.Bearing.x * scale;
		float ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		glm::vec4 vertices[4];

		vertices[0] = { -0.5f,      -0.5f + h,	0.0f, 1.0f };
		vertices[1] = { 0.5f,      -0.5f,		0.0f, 1.0f };
		vertices[2] = { 0.5f + w,  0.5f,		0.0f, 1.0f };
		vertices[3] = { -0.5f + w, 0.5f + h,	0.0f, 1.0f };
		
		textData.TextureID = ch.TextureID;

		/*std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create((float*)&vertices, sizeof(vertices)));

		vertexBuffer->SetLayout({
			{ShaderDataType::Float2,"a_Position"},
			{ShaderDataType::Float2,"a_TexCoord"},
			{ShaderDataType::Float4,"a_Color"}
			});
		vertexArray->AddVertexBuffer(vertexBuffer);
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		vertexArray->SetIndexBuffer(indexBuffer);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		vertexArray->Bind();

		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);*/

		for(auto vertex:vertices)
			textData.Vertices.push_back(vertex);

		position.x += (ch.Advance >> 6) * scale;
	}
	

	return textData;

	/*glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);*/
}
