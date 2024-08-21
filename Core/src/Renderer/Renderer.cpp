#include "pch.h"
#include "Renderer.h"
#include "Renderer.h"

#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "RendererAPI.h"
#include "OrthographicCameraController.h"
#include "FrameBuffer.h"

#include <glad/glad.h>
#include <glm/gtx/matrix_decompose.hpp>

#include "RenderCommand.h"
#include "Texture.h"
#include "Application.h"
#include "TextRenderer.h"
#include "UIRenderer.h"

std::unique_ptr<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();

struct CircleVertex
{
	glm::vec3 WorldPosition;
	glm::vec3 LocalPosition;
	glm::vec4 Color;
	float Thickness;
	float Fade;

	int EntityID;
};

struct QuadVertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec4 Color;
	float TexIndex;

	int EntityID;
};

struct LineVertex
{
	glm::vec3 Position;
	glm::vec4 Color;

	int EntityID;
};

struct RendererData
{
	static const uint32_t MaxQuads = 20000;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;
	static const uint32_t MaxTextureSlots = 32;

	std::shared_ptr<VertexArray> TriangleVertexArray;
	std::shared_ptr<VertexBuffer> TriangleVertexBuffer;

	std::shared_ptr<VertexArray> QuadVertexArray;
	std::shared_ptr<VertexBuffer> QuadVertexBuffer;
	std::shared_ptr<Shader> QuadShaderFlat;
	std::shared_ptr<Shader> QuadShaderTexture;

	std::shared_ptr<VertexArray> LineVertexArray;
	std::shared_ptr<VertexBuffer> LineVertexBuffer;
	std::shared_ptr<Shader> LineShader;

	std::shared_ptr<VertexArray> CircleVertexArray;
	std::shared_ptr<VertexBuffer> CircleVertexBuffer;
	std::shared_ptr<Shader> CircleShader;

	uint32_t TriangleIndexCount = 0;
	QuadVertex* TriangleVertexBufferBase = nullptr;
	QuadVertex* TriangleVertexBufferPtr = nullptr;

	uint32_t QuadIndexCount = 0;
	QuadVertex* QuadVertexBufferBase = nullptr;
	QuadVertex* QuadVertexBufferPtr = nullptr;

	uint32_t LineVertexCount = 0;
	LineVertex* LineVertexBufferBase = nullptr;
	LineVertex* LineVertexBufferPtr = nullptr;

	uint32_t CircleIndexCount = 0;
	CircleVertex* CircleVertexBufferBase = nullptr;
	CircleVertex* CircleVertexBufferPtr = nullptr;

	glm::vec4 QuadVertexPositions[4];
	glm::vec4 TriangleVertexPositions[3];

	std::shared_ptr<VertexArray> FrameBufferVertexArray;
	std::shared_ptr<VertexBuffer> FrameBufferVertexBuffer;
	std::shared_ptr<Shader> FrameBufferShader;


	std::vector<std::shared_ptr<Texture2D>> Textures;
	int	texs[32];

	glm::vec3 LightPos;

	//Statistics
	Renderer::Statistics stats;
	
};

static RendererData s_Data;

void Renderer::Init()
{
	auto& window = Application::Get().GetWindow();
	s_SceneData->UIViewProjectionMatrix = glm::ortho(-static_cast<float>(window.GetWidth()) / 2, static_cast<float>(window.GetWidth()) / 2, -static_cast<float>(window.GetHeight()) / 2, static_cast<float>(window.GetHeight()) / 2);

	for (int i = 0; i < 32; i++)
	{
		s_Data.texs[i] = i + 1;
	}

	//triangle init
	s_Data.TriangleVertexArray.reset(VertexArray::Create());

	s_Data.TriangleVertexBuffer.reset(VertexBuffer::Create(s_Data.MaxVertices * sizeof(glm::vec3)));
	s_Data.TriangleVertexBuffer->SetLayout({
		{ShaderDataType::Float3,"a_Position"},
		{ShaderDataType::Float2,"a_TexCoord"},
		{ShaderDataType::Float4,"a_Color"},
		{ShaderDataType::Float,"a_TexIndex"},
		{ShaderDataType::Float,"a_EntityID"}
		});

	s_Data.TriangleVertexArray->AddVertexBuffer(s_Data.TriangleVertexBuffer);

	s_Data.TriangleVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

	uint32_t* triangleIndices = new uint32_t[s_Data.MaxIndices];

	{
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 3)
		{
			triangleIndices[i + 0] = offset + 0;
			triangleIndices[i + 1] = offset + 1;
			triangleIndices[i + 2] = offset + 2;

			offset += 3;
		}
	}

	std::shared_ptr<IndexBuffer> tindexBuffer;
	tindexBuffer.reset(IndexBuffer::Create(triangleIndices, s_Data.MaxIndices));
	s_Data.TriangleVertexArray->SetIndexBuffer(tindexBuffer);

	delete[] triangleIndices;

	//quad init
	s_Data.QuadVertexArray.reset(VertexArray::Create());

	s_Data.QuadVertexBuffer.reset(VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex)));
	s_Data.QuadVertexBuffer->SetLayout({
		{ShaderDataType::Float3,"a_Position"},
		{ShaderDataType::Float2,"a_TexCoord"},
		{ShaderDataType::Float4,"a_Color"},
		{ShaderDataType::Float,"a_TexIndex"},
		{ShaderDataType::Float,"a_EntityID"}
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
		{ShaderDataType::Float4, "a_Color"},
		{ShaderDataType::Float, "a_EntityID"}
		});

	s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
	s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

	//Circle Init
	s_Data.CircleVertexArray.reset(VertexArray::Create());

	s_Data.CircleVertexBuffer.reset(VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex)));
	s_Data.CircleVertexBuffer->SetLayout({
		{ ShaderDataType::Float3, "a_WorldPosition" },
		{ ShaderDataType::Float3, "a_LocalPosition" },
		{ ShaderDataType::Float4, "a_Color"         },
		{ ShaderDataType::Float,  "a_Thickness"     },
		{ ShaderDataType::Float,  "a_Fade"          },
		{ ShaderDataType::Float,  "a_EntityID"      }
		});
	s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
	s_Data.CircleVertexArray->SetIndexBuffer(indexbuffer); // Use quad IB
	s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];

	//Text Renderer Init
	TextRenderer::Init();

	//framebuffer init
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	uint32_t indices[] = {
		0,1,2,0,2,3
	};
	s_Data.FrameBufferVertexArray.reset(VertexArray::Create());

	std::shared_ptr<VertexBuffer> vertexBuffer;
	vertexBuffer.reset(VertexBuffer::Create((float*)&quadVertices, sizeof(quadVertices)));

	BufferLayout layout = {
			{ShaderDataType::Float2, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float4, "a_Color"}
	};
	vertexBuffer->SetLayout(layout);
	s_Data.FrameBufferVertexArray->AddVertexBuffer(vertexBuffer);

	std::shared_ptr<IndexBuffer> indexBuffer;
	indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	s_Data.FrameBufferVertexArray->SetIndexBuffer(indexBuffer);

	

	//shaders init
	s_Data.QuadShaderFlat.reset(new Shader("assets/shaders/FlatColor.glsl"));
	s_Data.LineShader.reset(new Shader("assets/shaders/Line.glsl"));
	s_Data.CircleShader.reset(new Shader("assets/shaders/Circle.glsl"));
	s_Data.FrameBufferShader.reset(new Shader("assets/shaders/screen.glsl"));

	s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	s_Data.TriangleVertexPositions[0] = { -0.5f,-0.5f,0.0f,1.0f };
	s_Data.TriangleVertexPositions[1] = { 0.5f,-0.5f,0.0f,1.0f };
	s_Data.TriangleVertexPositions[2] = { 0.0f,0.5f,0.0f,1.0f };
}

void Renderer::Shutdown()
{
}


void Renderer::BeginScene(OrthographicCamera& camera)
{
	s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

	StartBatch();
}

void Renderer::BeginScene()
{
	s_SceneData->ViewProjectionMatrix = s_SceneData->UIViewProjectionMatrix;

	StartBatch();
}

void Renderer::EndScene()
{
	Flush();
	TextRenderer::Flush();
}

void Renderer::Flush()
{
	//Binding textures
	for(int i=0;i<s_Data.Textures.size();i++)
	{
		s_Data.Textures[i]->Bind(i + 1);
	}

	if(s_Data.TriangleIndexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.TriangleVertexBufferPtr - (uint8_t*)s_Data.TriangleVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.TriangleVertexBufferBase, dataSize);

		s_Data.QuadShaderFlat->Bind();
		s_Data.QuadShaderFlat->SetIntArray("u_Textures", s_Data.texs);
		s_Data.QuadShaderFlat->SetMat4("projection", s_SceneData->ViewProjectionMatrix);
		RenderCommand::DrawIndexed(s_Data.TriangleVertexArray, s_Data.TriangleIndexCount);

		s_Data.stats.DrawCalls++;
	}

	if (s_Data.LineVertexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
		s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

		RenderCommand::SetLineWidth(2);
		s_Data.LineShader->Bind();
		s_Data.LineShader->SetMat4("projection", s_SceneData->ViewProjectionMatrix);
		RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
		s_Data.stats.DrawCalls++;
	}

	if (s_Data.QuadIndexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		s_Data.QuadShaderFlat->Bind();
		s_Data.QuadShaderFlat->SetVec4("pickingColor", glm::vec4(s_Data.QuadVertexBufferPtr->EntityID));
		s_Data.QuadShaderFlat->SetIntArray("u_Textures", s_Data.texs);
		s_Data.QuadShaderFlat->SetMat4("projection", s_SceneData->ViewProjectionMatrix);
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

		s_Data.stats.DrawCalls++;
	}

	if (s_Data.CircleIndexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
		s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

		s_Data.CircleShader->Bind();
		s_Data.CircleShader->SetMat4("projection", s_SceneData->ViewProjectionMatrix);
		RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
		s_Data.stats.DrawCalls++;
	}
}

void Renderer::DrawTriangle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
		* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	DrawTriangle(transform, color);
}

void Renderer::DrawTriangle(const glm::mat4& transform, const glm::vec4& color, int id)
{
	constexpr size_t triangleVertexCount = 3;
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

	if (s_Data.TriangleIndexCount>= RendererData::MaxIndices)
		NextBatch();

	for (size_t i = 0; i < triangleVertexCount; i++)
	{
		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.TriangleVertexPositions[i];
		s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexIndex = 0;
		s_Data.QuadVertexBufferPtr->EntityID = id;
		s_Data.QuadVertexBufferPtr++;
	}
	s_Data.TriangleIndexCount+= 3;

	s_Data.stats.QuadCount++;
}

void Renderer::StartBatch()
{
	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

	s_Data.TriangleIndexCount = 0;
	s_Data.TriangleVertexBufferPtr= s_Data.TriangleVertexBufferBase;

	s_Data.CircleIndexCount = 0;
	s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

	s_Data.LineVertexCount = 0;
	s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

	TextRenderer::StartBatch();
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


void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,int id)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
		* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	DrawQuad(transform, color, id);
}

void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture2D>& texture,int id)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z)) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

	DrawQuad(transform, texture, id);
}

void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color,int id)
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
		s_Data.QuadVertexBufferPtr->TexIndex = 0;
		s_Data.QuadVertexBufferPtr->EntityID = id;
		s_Data.QuadVertexBufferPtr++;
	}
	s_Data.QuadIndexCount += 6;

	s_Data.stats.QuadCount++;
}

void Renderer::DrawQuad(const glm::mat4& transform, std::shared_ptr<Texture2D>& texture, int id)
{
	constexpr size_t quadVertexCount = 4;
	float textureIndex = 0.0f;
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

	if (s_Data.QuadIndexCount >= RendererData::MaxIndices)
		NextBatch();

	for (size_t i = 0; i < s_Data.Textures.size(); i++)
	{
		if (s_Data.Textures[i] == texture)
		{
			textureIndex = i;
		}
	}
	if (textureIndex == 0)
	{
		s_Data.Textures.push_back(texture);
		textureIndex = s_Data.Textures.size() - 1;
;	}

	

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
		s_Data.QuadVertexBufferPtr->Color = { 0.0f,0.0f,0.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->EntityID = id;
		s_Data.QuadVertexBufferPtr++;
	}

	s_Data.QuadIndexCount += 6;

	s_Data.stats.QuadCount++;
}

void Renderer::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int id)
{
	for (size_t i = 0; i < 4; i++)
	{
		s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
		s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
		s_Data.CircleVertexBufferPtr->Color = color;
		s_Data.CircleVertexBufferPtr->Thickness = thickness;
		s_Data.CircleVertexBufferPtr->Fade = fade;
		s_Data.CircleVertexBufferPtr->EntityID = id;
		s_Data.CircleVertexBufferPtr++;
	}

	
	s_Data.CircleIndexCount += 6;

	s_Data.stats.QuadCount++;
}

void Renderer::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int id)
{
	s_Data.LineVertexBufferPtr->Position = p0;
	s_Data.LineVertexBufferPtr->Color = color;
	s_Data.LineVertexBufferPtr++;

	s_Data.LineVertexBufferPtr->Position = p1;
	s_Data.LineVertexBufferPtr->Color = color;
	s_Data.LineVertexBufferPtr->EntityID = id;
	s_Data.LineVertexBufferPtr++;

	s_Data.LineVertexCount += 2;
}

void Renderer::DrawFrameBuffer(std::shared_ptr<FrameBuffer> buffer)
{
	glDisable(GL_DEPTH_TEST);
	Application& app = Application::Get();
	/*glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_2D, buffer->GetColorAttachmentRendererID());*/
	s_Data.FrameBufferShader->Bind();
	s_Data.FrameBufferShader->SetInt("screenTexture", 20);
	s_Data.FrameBufferShader->SetVec2("resoulation", glm::vec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight()));
	buffer->BindVertexArray();
	glDisable(GL_BLEND);
	glDrawElements(GL_TRIANGLES, buffer->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	glEnable(GL_BLEND);
}

void Renderer::RenderText(const std::string& text,const glm::vec3& position,float scale,const glm::vec4& color)
{
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f } };
	
	TextRenderer::RenderText(text, position, scale, color);
}

void Renderer::ResetStats()
{
	memset(&s_Data.stats, 0, sizeof(Statistics));
}

Renderer::Statistics& Renderer::GetStats()
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
