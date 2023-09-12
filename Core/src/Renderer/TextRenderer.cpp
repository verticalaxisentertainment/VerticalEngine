#include "pch.h"
#include "TextRenderer.h"
#include "Buffer.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "Renderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>

struct Character {
	std::shared_ptr<Texture> Texture;	// ID handle of the glyph texture
	glm::ivec2   Size;					// Size of glyph
	glm::ivec2   Bearing;				// Offset from baseline to left/top of glyph
	unsigned int Advance;				// Horizontal offset to advance to next glyph
};
std::map<GLchar, Character> Characters;

struct TextVertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec4 Color;
	float TexIndex;
};

struct TextRendererData
{
	static const uint32_t MaxQuads = 20000;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;
	static const uint32_t MaxTextureSlots = 32;

	std::shared_ptr<VertexArray> TextVertexArray;
	std::shared_ptr<VertexBuffer> TextVertexBuffer;
	std::shared_ptr<Shader> TextShader;

	uint32_t TextVertexCount = 0;
	TextVertex* TextVertexBufferBase = nullptr;
	TextVertex* TextVertexBufferPtr = nullptr;

	glm::vec4 QuadVertexPositions[4];

	std::vector<std::shared_ptr<Texture>> Textures;
	int	texs[32];
};

static TextRendererData s_Data;

void TextRenderer::Init()
{
	for (int i = 0; i < 32; i++)
	{
		s_Data.texs[i] = i;
	}

	uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

	uint32_t offset = 0;
	for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}

	std::shared_ptr<IndexBuffer> indexbuffer;
	indexbuffer.reset(IndexBuffer::Create(quadIndices, s_Data.MaxIndices));
	delete[] quadIndices;

	//Text Init
	s_Data.TextVertexArray.reset(VertexArray::Create());

	s_Data.TextVertexBuffer.reset(VertexBuffer::Create(s_Data.MaxVertices * sizeof(TextVertex)));
	s_Data.TextVertexBuffer->SetLayout({
		{ShaderDataType::Float3,"a_Position"},
		{ShaderDataType::Float2,"a_TexCoord"},
		{ShaderDataType::Float4,"a_Color"},
		{ShaderDataType::Float,"a_TexIndex"}
		});
	s_Data.TextVertexArray->AddVertexBuffer(s_Data.TextVertexBuffer);
	s_Data.TextVertexArray->SetIndexBuffer(indexbuffer); // TODO: change the index buffer to be complatible with batch rendering ,now it's using framebuffer's
	s_Data.TextVertexBufferBase = new TextVertex[s_Data.MaxVertices];

	//shader init
	s_Data.TextShader.reset(new Shader("assets/shaders/Text.glsl"));

	s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };


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

		std::shared_ptr<Texture> testTexture;
		testTexture.reset(Texture2D::Create(face->glyph->bitmap.width, face->glyph->bitmap.rows, LINEAR, CLAMPTOEDGE, RED_INTEGER, face->glyph->bitmap.buffer));
		
		// now store character for later use
		Character character = {
			testTexture,
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

void TextRenderer::StartBatch()
{
	s_Data.TextVertexCount = 0;
	s_Data.TextVertexBufferPtr = s_Data.TextVertexBufferBase;
}

void TextRenderer::Flush()
{

	if (s_Data.TextVertexCount)
	{
		//binding textures
		for (int i = 0; i < s_Data.Textures.size(); i++)
			s_Data.Textures[i]->Bind(i);

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.TextVertexBufferPtr - (uint8_t*)s_Data.TextVertexBufferBase);
		s_Data.TextVertexBuffer->SetData(s_Data.TextVertexBufferBase, dataSize);

		s_Data.TextShader->Bind();
		s_Data.TextShader->SetMat4("projection", Renderer::s_SceneData->ViewProjectionMatrix);
		s_Data.TextShader->SetIntArray("u_Textures", s_Data.texs);

		RenderCommand::DrawIndexed(s_Data.TextVertexArray, s_Data.TextVertexCount);
		Renderer::GetStats().DrawCalls++;
	}

	s_Data.Textures.clear();
}

TextData TextRenderer::RenderText(const std::string& text, glm::vec3 position, const float& scale,const glm::vec4& color)
{
	TextData textData;
	std::vector<int> letterIndexes;

	std::string::const_iterator c;
	float texIndex = -1.0f;

	for (c = text.begin(); c != text.end(); c++)
	{
		Renderer::GetStats().QuadCount++;


		Character ch = Characters[*c];
		float xpos = position.x + ch.Bearing.x * scale;
		float ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		glm::vec4 vertices[4];

		vertices[0] = {xpos ,   ypos + h,	0.0f, 1.0f };
		vertices[1] = {xpos  ,  ypos,		0.0f, 1.0f };
		vertices[2] = {xpos + w,ypos,		0.0f, 1.0f };
		vertices[3] = {xpos + w,ypos + h,	0.0f, 1.0f };
		

		textData.Texture.push_back(ch.Texture);

		if (s_Data.Textures.size() == 0)
		{
			s_Data.Textures.push_back(ch.Texture);
			letterIndexes.push_back(texIndex + 1);
		}
		else
		{
			for (int i = 0; i < s_Data.Textures.size(); i++)
			{
				if (ch.Texture == s_Data.Textures[i])
				{
					texIndex = i;
					letterIndexes.push_back(texIndex);
					break;
				}
			}
			if (texIndex == -1.0f)
			{
				s_Data.Textures.push_back(ch.Texture);
				letterIndexes.push_back(s_Data.Textures.size() - 1);
			}

		}


		for(auto vertex:vertices)
			textData.Vertices.push_back(vertex);

		position.x += (ch.Advance >> 6) * scale;
		texIndex = -1.0f;
	}

	texIndex = 0.0f;

	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f } };
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), { 1.0f,1.0f,1.0f });

	for (size_t i = 0; i < 4 * text.size(); i++)
	{
		if (i != 0 && i % 4 == 0)
			texIndex++;

		glm::vec3 pos = transform * textData.Vertices[i];
		s_Data.TextVertexBufferPtr->Position = { pos.x, pos.y,position.z };
		s_Data.TextVertexBufferPtr->TexCoord = textureCoords[i % 4];
		s_Data.TextVertexBufferPtr->Color = color;
		s_Data.TextVertexBufferPtr->TexIndex = letterIndexes[texIndex];
		s_Data.TextVertexBufferPtr++;

	}

	s_Data.TextVertexCount += 6 * text.size();


	return textData;
}


