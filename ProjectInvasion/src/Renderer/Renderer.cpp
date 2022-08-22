#include "pch.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "RendererAPI.h"

#include <glad/glad.h>

#include "RenderCommand.h"


struct QuadVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	float TexIndex;
	float TilingFactor;

	// Editor-only
	int EntityID;
};

struct RendererData
{
	static const uint32_t MaxQuads = 20000;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;
	static const uint32_t MaxTextureSlots = 32;

	std::shared_ptr<VertexArray> QuadVertexArray;
	std::shared_ptr<VertexBuffer> QuadVertexBuffer;

	std::shared_ptr<Shader> QuadShaderFlat;
	std::shared_ptr<Shader> QuadShaderTexture;
};

static RendererData s_Data;

void Renderer::Init()
{
	RenderCommand::Init();
	float vertices[]{
		// positions   // texCoords  //colors
			-0.5f,  0.5f,  0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			 0.5f, -0.5f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f,  1.0f, 1.0f,   1.0f, 0.0f, 0.0f
	};

	uint32_t indices[]{ 0,1,2,0,2,3 };

	s_Data.QuadVertexArray.reset(VertexArray::Create());

	s_Data.QuadVertexBuffer.reset(VertexBuffer::Create((float*)&vertices, sizeof(vertices)));
	s_Data.QuadVertexBuffer->SetLayout({
		{ShaderDataType::Float2,"a_Position"},
		{ShaderDataType::Float2,"a_TexCoord"},
		{ShaderDataType::Float3,"a_Color"}
		});

	s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

	std::shared_ptr<IndexBuffer> indexbuffer;
	indexbuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	s_Data.QuadVertexArray->SetIndexBuffer(indexbuffer);

	s_Data.QuadShaderFlat.reset(new Shader("assets/shaders/FlatColor.glsl"));
	s_Data.QuadShaderTexture.reset(new Shader("assets/shaders/Texture.glsl"));
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f)) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

	DrawQuad(transform, color);
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture2D>& texture)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f)) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

	DrawQuad(transform, texture);
}

void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
{
	s_Data.QuadShaderFlat->Bind();
	s_Data.QuadShaderFlat->SetMat4("model", transform);
	glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
	s_Data.QuadShaderFlat->SetMat4("projection", projection);
	/*s_Data.QuadVertexArray->Bind();
	glDrawElements(GL_TRIANGLES, s_Data.QuadVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);*/
	RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
}

void Renderer::DrawQuad(const glm::mat4& transform, std::shared_ptr<Texture2D>& texture)
{
	texture->Bind(1);
	s_Data.QuadShaderTexture->Bind();
	s_Data.QuadShaderTexture->SetMat4("model", transform);
	glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
	s_Data.QuadShaderTexture->SetMat4("projection", projection);
	s_Data.QuadShaderTexture->SetInt("tex", 1);
	s_Data.QuadShaderTexture->SetInt("tex2", 1);

	/*s_Data.QuadVertexArray->Bind();
	glDrawElements(GL_TRIANGLES, s_Data.QuadVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);*/
	RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
}
