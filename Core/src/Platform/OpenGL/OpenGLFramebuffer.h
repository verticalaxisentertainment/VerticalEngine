#pragma once

#include "Renderer/FrameBuffer.h"
#include "Renderer/VertexArray.h"

class OpenGLFramebuffer:public FrameBuffer
{
public:
	OpenGLFramebuffer(const FrameBufferSpecification& spec);
	virtual ~OpenGLFramebuffer();

	virtual void Bind() override;
	virtual void UnBind() override;

	virtual void BindVertexArray() override;

	virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return frameVA->GetIndexBuffer(); }

	virtual void Resize(float width, float height) override;
	virtual float* ReadPixel(uint32_t attachmentIndex, int x, int y) override;

	void Invalidate();

	virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }
	virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
private:
	uint32_t m_RendererID;
	uint32_t m_ColorAttachment, m_DepthAttachment;
	uint32_t m_RenderBuffer;

	FrameBufferSpecification m_Specification;
	std::shared_ptr<VertexArray> frameVA;
};
