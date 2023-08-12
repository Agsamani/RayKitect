#pragma once

#include <random>
#include "glm/glm.hpp"

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

	static glm::vec3 Vec3() {
		return glm::vec3(Float(), Float(), Float());
	}

	static glm::vec3 InUnitSphere() {
		return normalize(2.0f * Vec3() - 1.0f);
	}

private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

namespace Utils {
	uint32_t CastToRGBA(const glm::vec4& value);
};