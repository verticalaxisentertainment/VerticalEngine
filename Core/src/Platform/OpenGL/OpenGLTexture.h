﻿#pragma once

#include "Renderer/Texture.h"
#include <glad/glad.h>

class OpenGLTexture2D :public Texture2D
{
public:
	OpenGLTexture2D(uint32_t width, uint32_t height, TextureSpecification Specification, unsigned char* Buffer);
	OpenGLTexture2D(const std::string& path);
	OpenGLTexture2D(FrameBuffer* framebuffer);

	virtual ~OpenGLTexture2D();

	virtual uint32_t GetHeight() const override { return m_Width; }
	virtual uint32_t GetWidth() const override { return m_Height; }
	virtual uint32_t GetRendererID() const override { return m_RendererID; }

	virtual const std::string& GetPath() const override { return m_Path; }

	virtual void SetData(void* data, uint32_t size) override;
	virtual void UpdateTexture(const std::string& path) override;

	virtual void Bind(uint32_t slot) const override;

	virtual bool IsLoaded() const override { return m_IsLoaded; }

	virtual bool operator==(const Texture& other) const override
	{
		return m_RendererID == other.GetRendererID();
	}
private:
	std::string m_Path;
	bool m_IsLoaded;
	uint32_t m_Width, m_Height;
	uint32_t m_RendererID;
	GLenum m_InternalFormat, m_DataFormat;
};
