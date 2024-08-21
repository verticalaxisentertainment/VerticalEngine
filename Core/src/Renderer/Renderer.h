﻿#pragma once

#include "RendererAPI.h"
#include "Texture.h"
#include "OrthographicCamera.h"
#include "Core.h"
#include "FrameBuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader;

class _API Renderer
{
public:
	inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
public:
	static void Init();
	static void Shutdown();

	static void BeginScene(OrthographicCamera& camera);
	static void BeginScene();
	static void EndScene();
	static void Flush();

	//Primitives
	static void DrawTriangle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
	static void DrawTriangle(const glm::mat4& transform, const glm::vec4& color,int id = 1);

	static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int id = 1);
	static void DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture2D>& texture, int id = 1);
	//static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

	static void DrawQuad(const glm::mat4& transform, const glm::vec4& color,int id = 1);
	static void DrawQuad(const glm::mat4& transform, std::shared_ptr<Texture2D>& texture,int id = 1);


	static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
	static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

	static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f,int id = 2);

	static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color,int id = 1);

	static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
	static void DrawRect(const glm::mat4& transform, const glm::vec4& color);

	static void DrawFrameBuffer(std::shared_ptr<FrameBuffer> buffer);

	static void RenderText(const std::string& text,const glm::vec3& position,float scale,const glm::vec4& color);

	struct Statistics
	{
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;

		uint32_t GetTotalVertexCount() { return QuadCount * 4; }
		uint32_t GetTotalIndexCount() { return QuadCount * 6; }
	};

	static void ResetStats();
	static Statistics& GetStats();

	static void WireframeMode(bool on);
	static void SetDepthTest(bool on);


private:
	struct SceneData
	{
		glm::mat4 ViewProjectionMatrix;
		glm::mat4 UIViewProjectionMatrix;
	};


	static std::unique_ptr<SceneData> s_SceneData;
	static std::unique_ptr<SceneData>& GetSceneData() { return s_SceneData; }
	friend class TextRenderer;
	friend class UIRenderer;
private:
	static void StartBatch();
	static void NextBatch();

	static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
};
