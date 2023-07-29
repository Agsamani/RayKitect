#pragma once

#include "Program.h"
#include "Shader.h"
#include "Texture.h"


class ComputeShader
{
public:
	ComputeShader(std::string path);
	~ComputeShader();

	void Dispatch(uint32_t numGroupX, uint32_t numGroupY, uint32_t numGroupZ);

	// Temp
	const Program& GetProgram() const { return program; }
private:
	Program program;
	Shader shader;
};

