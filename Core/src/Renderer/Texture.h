#pragma once
#include "Core.h"

enum TextureOption
{
	LINEAR = 0, REPEAT, CLAMPTOEDGE
};

enum TextureFormat
{
	RED_INTEGER,RGBA
};

struct TextureSpecification
{
	TextureOption MinFilter;
	TextureOption MagFilter;
	TextureOption WrapSFilter;
	TextureOption WrapTFilter;
	TextureFormat Format;
};

class Texture
{
public:
	virtual ~Texture() = default;

	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetHeight() const = 0;
	virtual uint32_t GetRendererID() const = 0;

	virtual const std::string& GetPath() const = 0;

	virtual void SetData(void* data, uint32_t size) = 0;
	virtual void UpdateTexture(const std::string& path) = 0;

	virtual void Bind(uint32_t slot = 0) const = 0;

	virtual bool IsLoaded() const = 0;

	virtual bool operator==(const Texture& other) const = 0;
};

class _API Texture2D :public Texture
{
public:
	static Texture2D* Create(uint32_t width, uint32_t height, TextureSpecification specification = { LINEAR, LINEAR, REPEAT, REPEAT,RGBA }, unsigned char* buffer = nullptr);
	static Texture2D* Create(const std::string& path);
};
