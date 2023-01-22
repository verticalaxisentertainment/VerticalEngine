#include "pch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None: INFO("Currently not supported RendererAPI"); return nullptr;
	case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
	}

	ERROR("Unknown RendererAPI");
	return nullptr;
}

VertexBuffer* VertexBuffer::Create(uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None: INFO("Currently not supported RendererAPI"); return nullptr;
	case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(size);
	}

	ERROR("Unknown RendererAPI");
	return nullptr;
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None: INFO("Currently not supported RendererAPI"); return nullptr;
	case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, count);
	}

	ERROR("Unknown RendererAPI");
	return nullptr;
}
