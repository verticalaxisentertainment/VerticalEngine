#include "pch.h"
#include "Renderer.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "RendererAPI.h"

#include <glad/glad.h>

#include "RenderCommand.h"

std::unique_ptr<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();

struct QuadVertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec4 Color;
};

struct LineVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
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

	std::shared_ptr<VertexArray> LineVertexArray;
	std::shared_ptr<VertexBuffer> LineVertexBuffer;
	std::shared_ptr<Shader> LineShader;

	uint32_t QuadIndexCount = 0;
	QuadVertex* QuadVertexBufferBase = nullptr;
	QuadVertex* QuadVertexBufferPtr = nullptr;

	uint32_t LineVertexCount = 0;
	LineVertex* LineVertexBufferBase = nullptr;
	LineVertex* LineVertexBufferPtr = nullptr;

	glm::vec4 QuadVertexPositions[4];

	//Statistics
	Renderer::Statistics stats;
	
};

static RendererData s_Data;

void Renderer::Init()
{
	RenderCommand::Init();

	s_Data.QuadVertexArray.reset(VertexArray::Create());

	s_Data.QuadVertexBuffer.reset(VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex)));
	s_Data.QuadVertexBuffer->SetLayout({
		{ShaderDataType::Float3,"a_Position"},
		{ShaderDataType::Float2,"a_TexCoord"},
		{ShaderDataType::Float4,"a_Color"}
		});

	s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

	s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

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
	s_Data.QuadVertexArray->SetIndexBuffer(indexbuffer);
	delete[] quadIndices;



	//Line init
	s_Data.LineVertexArray.reset(VertexArray::Create());

	s_Data.LineVertexBuffer.reset(VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex)));
	s_Data.LineVertexBuffer->SetLayout({
		{ShaderDataType::Float3,"a_Position"},
		{ShaderDataType::Float4, "a_Color"}
		});

	s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
	s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

	//shaders init
	s_Data.QuadShaderFlat.reset(new Shader("assets/shaders/FlatColor.glsl"));
	s_Data.QuadShaderTexture.reset(new Shader("assets/shaders/Texture.glsl"));
	s_Data.LineShader.reset(new Shader("assets/shaders/Line.glsl"));

	s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
}

void Renderer::BeginScene(OrthographicCamera& camera)
{
	s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	
	
	ResetStats();
	StartBatch();
}

void Renderer::EndScene()
{
	Flush();
}

void Renderer::Flush()
{
	if (s_Data.LineVertexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
		s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

		RenderCommand::SetLineWidth(2);
		s_Data.LineShader->Bind();
		s_Data.LineShader->SetMat4("projection", Renderer::s_SceneData->ViewProjectionMatrix);
		RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
		s_Data.stats.DrawCalls++;
	}

	if (s_Data.QuadIndexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		s_Data.QuadShaderFlat->Bind();
		s_Data.QuadShaderFlat->SetMat4("projection", Renderer::s_SceneData->ViewProjectionMatrix);
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.stats.DrawCalls++;
	}
}

void Renderer::StartBatch()
{
	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

	s_Data.LineVertexCount = 0;
	s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
}

void Renderer::NextBatch()
{
	Flush();
	StartBatch();
}

void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
	const glm::mat4& transform)
{
	shader->Bind();
	shader->SetMat4("projection", s_SceneData->ViewProjectionMatrix);
	shader->SetMat4("model", transform);

	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}


void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
		* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	DrawQuad(transform, color);
}

void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture2D>& texture)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z)) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

	DrawQuad(transform, texture);
}

void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
{
	constexpr size_t quadVertexCount = 4;
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

	if (s_Data.QuadIndexCount >= RendererData::MaxIndices)
		NextBatch();

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
		s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr++;
	}

	s_Data.QuadIndexCount += 6;

	s_Data.stats.QuadCount++;
}

void Renderer::DrawQuad(const glm::mat4& transform, std::shared_ptr<Texture2D>& texture)
{
	constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= RendererData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = { 0.0f,0.0f,0.0f,1.0f };
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
}

void Renderer::DrawLine(const glm::vec3& p0,const glm::vec3& p1, const glm::vec4& color)
{
	s_Data.LineVertexBufferPtr->Position = p0;
	s_Data.LineVertexBufferPtr->Color = color;
	s_Data.LineVertexBufferPtr++;

	s_Data.LineVertexBufferPtr->Position = p1;
	s_Data.LineVertexBufferPtr->Color = color;
	s_Data.LineVertexBufferPtr++;

	s_Data.LineVertexCount += 2;
}

void Renderer::ResetStats()
{
	memset(&s_Data.stats, 0, sizeof(Statistics));
}

Renderer::Statistics Renderer::GetStats()
{
	return s_Data.stats;
}

void Renderer::WireframeMode(bool on)
{
	if (on)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
