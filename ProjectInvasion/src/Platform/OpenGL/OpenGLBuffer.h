#pragma once

#include "Renderer/Buffer.h"
#include <array>
#include "Renderer/VertexArray.h"

class OpenGLVertexBuffer:public VertexBuffer
{
public:
	OpenGLVertexBuffer(float* vertices, uint32_t size);
	virtual ~OpenGLVertexBuffer();

	virtual void Bind() const override;
	virtual void UnBind() const override;

	virtual BufferLayout& GetLayout() override { return m_Layout; }
	virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
private:
	uint32_t m_RendererID;
	BufferLayout m_Layout;
};

class OpenGLIndexBuffer :public IndexBuffer
{
public:
	OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
	virtual ~OpenGLIndexBuffer();

	virtual void Bind() const override;
	virtual void UnBind() const override;

	virtual uint32_t GetCount() const override { return m_Count; }
private:
	uint32_t m_RendererID;
	uint32_t m_Count;
};

class OpenGLFrameBuffer :public FrameBuffer
{
public:
	OpenGLFrameBuffer();
	virtual ~OpenGLFrameBuffer();

	virtual void Bind() const override;
	virtual void BindVertexArray() const override;
	virtual void UnBind() const override;

	virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return frameVA->GetIndexBuffer(); }

private:
	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	unsigned int rbo;

	std::shared_ptr<VertexArray> frameVA;
};
