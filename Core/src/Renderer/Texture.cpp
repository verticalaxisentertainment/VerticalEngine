#include "pch.h"
#include "Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer/Renderer.h"

Texture2D* Texture2D::Create(uint32_t width, uint32_t height)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None: INFO("Currently not supported RendererAPI"); return nullptr;
	case RendererAPI::API::OpenGL: return new OpenGLTexture2D(width, height);
	}

	ERROR("Unknown RendererAPI");
	return nullptr;
}

Texture2D* Texture2D::Create(const std::string& path)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None: INFO("Currently not supported RendererAPI"); return nullptr;
	case RendererAPI::API::OpenGL: return new OpenGLTexture2D(path);
	}

	ERROR("Unknown RendererAPI");
	return nullptr;
}
