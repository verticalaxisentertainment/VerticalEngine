#include "pch.h"
#include "OpenGLFramebuffer.h"
#include "Renderer/RenderCommand.h"

#include <glad/glad.h>
#include "Application.h"

OpenGLFramebuffer::OpenGLFramebuffer(const FrameBufferSpecification& spec)
	:m_Specification(spec)
{
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	uint32_t indices[] = {
		0,1,2,0,2,3
	};
	frameVA.reset(VertexArray::Create());

	std::shared_ptr<VertexBuffer> vertexBuffer;
	vertexBuffer.reset(VertexBuffer::Create((float*)&quadVertices, sizeof(quadVertices)));

	BufferLayout layout = {
			{ShaderDataType::Float2, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
	};
	vertexBuffer->SetLayout(layout);
	frameVA->AddVertexBuffer(vertexBuffer);

	std::shared_ptr<IndexBuffer> indexBuffer;
	indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	frameVA->SetIndexBuffer(indexBuffer);


	glGenFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	
	glGenTextures(1, &m_ColorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

	glGenTextures(1, &colorAttachmnetTest);
	glBindTexture(GL_TEXTURE_2D, colorAttachmnetTest );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_Specification.Width, m_Specification.Height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorAttachmnetTest , 0);

	glGenTextures(1, &m_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);


	const GLenum buffers[2]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(2, buffers);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
}

void OpenGLFramebuffer::Bind()
{
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	//RenderCommand::Clear();
}

void OpenGLFramebuffer::BindVertexArray()
{
	frameVA->Bind();
}

void OpenGLFramebuffer::Resize(float width, float height)
{
	m_Specification.Width = width; m_Specification.Height = height;

	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

	glBindTexture(GL_TEXTURE_2D, colorAttachmnetTest );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_Specification.Width, m_Specification.Height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorAttachmnetTest , 0);

	glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

	const GLenum buffers[3]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT2,GL_DEPTH_ATTACHMENT };
	glDrawBuffers(3, buffers);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

float* OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	float* pixelData=new float[4];
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, pixelData);
	return pixelData;
}

int OpenGLFramebuffer::ReadPixelInt(uint32_t attachmentIndex, int x, int y)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	int pixelData;
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
	return pixelData;
}

void OpenGLFramebuffer::UnBind()
{
	glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
