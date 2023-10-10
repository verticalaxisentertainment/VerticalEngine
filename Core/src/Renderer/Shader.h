#pragma once

#include "Core.h"
#include <string>
#include <glm/glm.hpp>


struct ShaderProps
{
	std::string ShaderPath;
	uint32_t RendererID;
};

static std::vector<ShaderProps> s_Shaders;

class _API Shader
{
public:
	Shader(const std::string& shaderPath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	void SetInt(const std::string& name,int value) const;
	void SetIntArray(const std::string& name,int* value) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;
	void SetVec2(const std::string& name, const glm::vec2& value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec4(const std::string& name, const glm::vec4& value) const;
	void SetFloat(const std::string& name, const float& value) const;

	static void CompileShader(const std::string& shaderPath,const uint32_t RendererID);
	static void ReCompileShaders();
private:
	uint32_t m_RendererID;
};
