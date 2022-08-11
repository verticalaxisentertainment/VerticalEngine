#pragma once

#include <string>

class Shader
{
public:
	Shader(const std::string& shaderPath);
	~Shader();

	void Bind() const;
	void UnBind() const;
private:
	uint32_t m_RendererID;
};
