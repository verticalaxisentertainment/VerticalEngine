#include "pch.h"
#include "Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer/Renderer.h"

Texture2D* Texture2D::Create(uint32_t width, uint32_t height)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None: std::cout << "Currently not supported RendererAPI" << std::endl; return nullptr;
	case RendererAPI::OpenGL: return new OpenGLTexture2D(width, height);
	}

	std::cout << "Unknown RendererAPI\n";
	return nullptr;
}

Texture2D* Texture2D::Create(const std::string& path)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None: std::cout << "Currently not supported RendererAPI" << std::endl; return nullptr;
	case RendererAPI::OpenGL: return new OpenGLTexture2D(path);
	}

	std::cout << "Unknown RendererAPI\n";
	return nullptr;
}
