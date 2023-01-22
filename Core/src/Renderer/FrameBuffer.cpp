#include "pch.h"
#include "FrameBuffer.h"
#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

FrameBuffer* FrameBuffer::Create(const FrameBufferSpecification& spec)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None: INFO("Currently not supported RendererAPI"); return nullptr;
	case RendererAPI::API::OpenGL: return new OpenGLFramebuffer(spec);
	}

	ERROR("Unknown RendererAPI");
	return nullptr;
}
