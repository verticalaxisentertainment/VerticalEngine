#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const std::string& shaderPath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	void SetInt(const std::string& name,int value) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;
	void SetVec2(const std::string& name, const glm::vec2& value) const;
private:
	uint32_t m_RendererID;
};
