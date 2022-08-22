#include "pch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

std::unique_ptr<RendererAPI> RendererAPI::Create()
{
	switch(s_API)
	{
		case RendererAPI::API::None: INFO("RendererAPI::None is currently not supported!");
		case RendererAPI::API::OpenGL: return std::make_unique<OpenGLRendererAPI>();
	}

	ERROR("Unknown RendererAPI!");
	return nullptr;
}
