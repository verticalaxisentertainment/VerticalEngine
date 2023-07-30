#include "pch.h"
#include "Image.h"

#include <stb_image.h>

Image::Image(const char* path)
	:m_Image(new ImageProps)
{
	m_Image->pixels=stbi_load(path, &m_Image->width, &m_Image->height, 0, 4); //rgba channels 
}

Image::~Image()
{
	stbi_image_free(m_Image->pixels);
	delete m_Image;
}
