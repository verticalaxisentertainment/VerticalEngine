#include "pch.h"
#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer/Renderer.h"

VertexArray* VertexArray::Create()
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None: INFO("Currently not supported RendererAPI"); return nullptr;
	case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
	}

	ERROR("Unknown RendererAPI");
	return nullptr;
}
