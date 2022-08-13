#include "pch.h"
#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer/Renderer.h"

VertexArray* VertexArray::Create()
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None: std::cout << "Currently not supported RendererAPI" << std::endl; return nullptr;
	case RendererAPI::OpenGL: return new OpenGLVertexArray();
	}

	std::cout << "Unknown RendererAPI\n";
	return nullptr;
}
