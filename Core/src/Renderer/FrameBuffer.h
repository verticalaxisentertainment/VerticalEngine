#pragma once

#include "Core.h"
#include "Renderer/Buffer.h"

struct FrameBufferSpecification
{
	uint32_t Width,Height;
	//FrameBuffer Format=
	uint32_t Samples = 1;

	bool SwapChain = false;
};


class _API FrameBuffer
{
public:
	virtual const FrameBufferSpecification& GetSpecification() const = 0;
	virtual uint32_t GetColorAttachmentRendererID() const = 0;

	virtual void Bind() = 0;
	virtual void UnBind() = 0;
	virtual void BindVertexArray() = 0;

	virtual void Resize(float width, float height) = 0;
	virtual float* ReadPixel(uint32_t attachmentIndex,int x, int y) = 0;

	virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

	static FrameBuffer* Create(const FrameBufferSpecification& spec);
};
