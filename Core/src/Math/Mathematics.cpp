#include "pch.h"
#include "Mathematics.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

glm::vec3 Math::ScreenToWorldPoint(const glm::vec3& pos, const glm::mat4& eye)
{
	glm::mat4 inverse = glm::inverse(eye);
	glm::vec4 tmp(1.0f);

	tmp.x = (pos.x / Application::Get().GetWindow().GetWidth()) * 2 - 1;
	tmp.y = 1 - (pos.y / Application::Get().GetWindow().GetHeight()) * 2;


	glm::vec4 obj = inverse * tmp;
	obj /= obj.w;

	glm::vec3 test = obj;
	return test;
}

float Math::RandomFloat(float range1, float range2)
{
	std::mt19937 s_RandomEngine;

	s_RandomEngine.seed(std::random_device()());
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	return (((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max()) * (range2 - range1)) + range1;
}

int Math::RandomInt(int range1, int range2)
{
	float tmp = (RandomFloat(range1, range2));
	if(tmp>=range2-0.5f)
	{
		return range2;
	}else
	{
		return static_cast<int>(tmp);
	}
}

float Math::Time()
{
	return glfwGetTime();
}
