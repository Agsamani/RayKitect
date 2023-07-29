#include <rktpch.h>
#include "Utils.h"

#include "GLFW/glfw3.h"

float Time::GetTime()
{
	return glfwGetTime();
}
	
std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;
