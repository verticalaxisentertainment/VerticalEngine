#pragma once

#include "RendererAPI.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Texture.h"
#include "OrthographicCamera.h"



class Renderer
{
public:
	inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
public:
	static void Init();
	static void Shutdown();

	static void BeginScene(OrthographicCamera& camera);
	static void EndScene();

	//Primitives
	static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
	static void DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture2D>& texture);
	//static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

	static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
	static void DrawQuad(const glm::mat4& transform, std::shared_ptr<Texture2D>& texture);


	static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
	static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

	static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f);

	static void DrawLine(const glm::vec3& p0,const glm::vec3& p1, const glm::vec4& color);

	static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
	static void DrawRect(const glm::mat4& transform, const glm::vec4& color);
private:
	struct SceneData
	{
		glm::mat4 ViewProjectionMatrix;
	};

	static std::unique_ptr<SceneData> s_SceneData;
};
