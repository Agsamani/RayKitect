#pragma once

#include <random>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include "core/Log.h";

class Time {
public:
	static float GetTime();
};

class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	static float Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

	static float Float(float min, float max)
	{
		return (max - min) * Float() + min;
	}

	static glm::vec3 Vec3() {
		return glm::vec3(Float(), Float(), Float());
	}

	static glm::vec3 InUnitSphere() {
		constexpr float PI = glm::pi<float>();
		float theta = 2 * PI * Float();
		float phi = glm::acos(1.0 - 2.0 * Float());

		float x = glm::sin(phi) * glm::cos(theta);
		float y = glm::sin(phi) * glm::sin(theta);
		float z = glm::cos(phi);

		return glm::vec3(x, y, z);
	}

private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

namespace Utils {
	uint32_t CastToRGBA(const glm::vec4& value);
};