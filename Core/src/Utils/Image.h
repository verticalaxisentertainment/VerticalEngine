#pragma once

struct ImageProps
{
	int width;
	int height;
	unsigned char* pixels;
};

class Image
{
public:
	Image(const char* path);
	~Image();

	void* GetProps() const { return m_Image; }
private:
	ImageProps* m_Image;
};

