#include "pch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

GLint GetTextureOption(TextureOption option)
{
    switch (option)
    {
        case LINEAR:        return GL_LINEAR;
        case REPEAT:        return GL_REPEAT;
        case CLAMPTOEDGE:   return GL_CLAMP_TO_EDGE;
    }

    return NULL;
}

GLenum GetTextureSlot(int slot)
{
    switch (slot)
    {
        case 1:     return GL_TEXTURE0;
        case 2:     return GL_TEXTURE1;
        case 3:     return GL_TEXTURE2;
        case 4:     return GL_TEXTURE3;
        case 5:     return GL_TEXTURE4;
        case 6:     return GL_TEXTURE5;
        case 7:     return GL_TEXTURE6;
        case 8:     return GL_TEXTURE7;
        case 9:     return GL_TEXTURE8;
        case 10:    return GL_TEXTURE9;
        case 11:    return GL_TEXTURE10;
        case 12:    return GL_TEXTURE11;
        case 13:    return GL_TEXTURE12;
        case 14:    return GL_TEXTURE13;
        case 15:    return GL_TEXTURE14;
        case 16:    return GL_TEXTURE15;
        case 17:    return GL_TEXTURE16;
        case 18:    return GL_TEXTURE17;
    }
    return GL_TEXTURE0;
}



OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, TextureOption MinMagFilter, TextureOption Wrap,TextureFormat Format,unsigned char* Buffer)
	:m_Width(width),m_Height(height)
{
    switch (Format)
    {
        case RED_INTEGER:
            m_InternalFormat = GL_RED;
            m_DataFormat = GL_RED;
            break;
        case RGBA:
	        m_InternalFormat = GL_RGBA8;
	        m_DataFormat = GL_RGBA;
            break;
    }

    // generate texture
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        m_InternalFormat,
        width,
        height,
        0,
        m_DataFormat,
        GL_UNSIGNED_BYTE,
        Buffer
    );

    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetTextureOption(Wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetTextureOption(Wrap));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetTextureOption(MinMagFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetTextureOption(MinMagFilter));

    glBindTexture(GL_TEXTURE_2D, 0);


    //glGenerateMipmap(GL_TEXTURE_2D);
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
    //glActiveTexture(GetTextureSlot(slot));
    glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
