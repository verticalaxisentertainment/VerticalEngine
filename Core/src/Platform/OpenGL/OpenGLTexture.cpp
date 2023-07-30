#include "pch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

GLenum GetTextureSlot(int slot)
{
    switch (slot)
    {
    case 1: return GL_TEXTURE0;
    case 2:return GL_TEXTURE1;
    case 3:return GL_TEXTURE2;
    case 4:return GL_TEXTURE3;
    case 5:return GL_TEXTURE4;
    case 6:return GL_TEXTURE5;
    case 7:return GL_TEXTURE6;
    case 8:return GL_TEXTURE7;
    case 9:return GL_TEXTURE8;
    case 10:return GL_TEXTURE9;
    }
    return GL_TEXTURE0;
}

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
	:m_Width(width),m_Height(height)
{
	m_InternalFormat = GL_RGBA8;
	m_DataFormat = GL_RGBA;

	glGenTextures(GL_TEXTURE_2D, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	:m_Path(path)
{
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        ERROR("Failed to load texture");
    }
    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::SetData(void* data, uint32_t size)
{
	uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
	glTexSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::UpdateTexture(const std::string& path)
{
    m_Path = path;
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        ERROR("Failed to load texture");
    }
    stbi_image_free(data);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
    glActiveTexture(GetTextureSlot(slot));
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
