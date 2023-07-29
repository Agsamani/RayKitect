#include "rktpch.h"
#include "ComputeShader.h"

#include <glad/glad.h>


// TODO : move uploading uniform here ( add bool to check if program is bounded )
ComputeShader::ComputeShader(std::string path)
	:shader(path, ShaderType::Compute)
{
	program.AttachShader(shader);
	program.LinkProgram();
}

ComputeShader::~ComputeShader()
{
}

void ComputeShader::Dispatch(uint32_t numGroupX, uint32_t numGroupY, uint32_t numGroupZ)
{
	program.UseProgram();
	glDispatchCompute(numGroupX, numGroupY, numGroupZ);
	// Temp
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}
