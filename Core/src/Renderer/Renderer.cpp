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
	glm::vec4 Color;
	glm::vec2 TexCoord;
	float TexIndex;
	float TilingFactor;

	// Editor-only
	int EntityID;
};

struct LineVertex
{
	glm::vec3 Position1;
	glm::vec3 Position2;
	glm::vec4 Color;


	//Editor-only
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

	std::shared_ptr<VertexArray> LineVertexArray;
	std::shared_ptr<VertexBuffer> LineVertexBuffer;
	std::shared_ptr<Shader> LineShader;
	LineVertex* lineVertexValues;
	//float LineVerctices[12];

	float lineVertices[12]
	{
		//position     //color
		0.0f,0.0f,0.0f,1.0f, 1.0f, 1.0f,
		1.0f,0.0f,0.0f,0.0f,1.0f,1.0f
	};

	std::shared_ptr<Shader> QuadShaderFlat;
	std::shared_ptr<Shader> QuadShaderTexture;

	//Statistics
	Renderer::Statistics stats;
	
};

static RendererData s_Data;

void Renderer::Init()
{
	RenderCommand::Init();

	//Quad init
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


	//Line init
	

	

	s_Data.LineVertexArray.reset(VertexArray::Create());
	s_Data.LineVertexBuffer.reset(VertexBuffer::Create(sizeof(s_Data.lineVertices)));
	s_Data.LineVertexBuffer->SetLayout({
		{ShaderDataType::Float3,"a_Position"},
		{ShaderDataType::Float3, "a_Color"}
		});

	s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);


	//shaders init
	s_Data.QuadShaderFlat.reset(new Shader("assets/shaders/FlatColor.glsl"));
	s_Data.QuadShaderTexture.reset(new Shader("assets/shaders/Texture.glsl"));
	s_Data.LineShader.reset(new Shader("assets/shaders/Line.glsl"));
}

void Renderer::BeginScene(OrthographicCamera& camera)
{
	s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	ResetStats();
}

void Renderer::EndScene()
{
}

void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z)) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

	DrawQuad(transform, color);
}

void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture2D>& texture)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z)) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

	DrawQuad(transform, texture);
}

void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
{
	s_Data.QuadShaderFlat->Bind();
	s_Data.QuadShaderFlat->SetMat4("model", transform);
	glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
	s_Data.QuadShaderFlat->SetMat4("projection", Renderer::s_SceneData->ViewProjectionMatrix);

	s_Data.stats.QuadCount++;
	s_Data.stats.DrawCalls++;
	RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
}

void Renderer::DrawQuad(const glm::mat4& transform, std::shared_ptr<Texture2D>& texture)
{
	texture->Bind(0);
	s_Data.QuadShaderTexture->Bind();
	s_Data.QuadShaderTexture->SetMat4("model", transform);
	glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
	s_Data.QuadShaderTexture->SetMat4("projection", Renderer::s_SceneData->ViewProjectionMatrix);
	s_Data.QuadShaderTexture->SetInt("tex", 0);
	s_Data.QuadShaderTexture->SetInt("tex2", 0);

	s_Data.stats.QuadCount++;
	s_Data.stats.DrawCalls++;
	RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
}

void Renderer::DrawLine(const glm::vec3& p0,const glm::vec3& p1, const glm::vec4& color)
{
	s_Data.lineVertices[0] = p0[0];
	s_Data.lineVertices[1] = p0[1];
	s_Data.lineVertices[2] = p0[2];

	s_Data.lineVertices[3] = color[0];
	s_Data.lineVertices[4] = color[1];
	s_Data.lineVertices[5] = color[2];

	s_Data.lineVertices[6] = p1[0];
	s_Data.lineVertices[7] = p1[1];
	s_Data.lineVertices[8] = p1[2];

	s_Data.lineVertices[9] = color[0];
	s_Data.lineVertices[10] = color[1];
	s_Data.lineVertices[11] = color[2];

	s_Data.LineVertexBuffer->SetData(&s_Data.lineVertices, sizeof(s_Data.lineVertices));
	s_Data.LineShader->Bind();
	s_Data.LineShader->SetMat4("projection", Renderer::s_SceneData->ViewProjectionMatrix);

	s_Data.stats.DrawCalls++;
	RenderCommand::DrawLines(s_Data.LineVertexArray);
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
