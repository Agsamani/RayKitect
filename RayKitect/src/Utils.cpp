#include <rktpch.h>
#include "Utils.h"

#include "GLFW/glfw3.h"

float Time::GetTime()
{
	return glfwGetTime();
}
	
std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

uint32_t Utils::CastToRGBA(const glm::vec4& value)
{
	uint32_t R = (uint8_t)(value.r * 255.0);
	uint32_t G = (uint8_t)(value.g * 255.0);
	uint32_t B = (uint8_t)(value.b * 255.0);
	uint32_t A = (uint8_t)(value.a * 255.0);

	return (A << 24) | (B << 16) | (G << 8) | (R);
}
