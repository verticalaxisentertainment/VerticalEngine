#include "pch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None: std::cout << "Currently not supported RendererAPI" << std::endl; return nullptr;
	case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
	}

	std::cout << "Unknown RendererAPI\n";
	return nullptr;
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None: std::cout << "Currently not supported RendererAPI" << std::endl; return nullptr;
	case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, count);
	}

	std::cout << "Unknown RendererAPI\n";
	return nullptr;
}

FrameBuffer* FrameBuffer::Create()
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None: std::cout << "Currently not supported RendererAPI" << std::endl; return nullptr;
	case RendererAPI::OpenGL: return new OpenGLFrameBuffer();
	}

	std::cout << "Unknown RendererAPI\n";
	return nullptr;
}
