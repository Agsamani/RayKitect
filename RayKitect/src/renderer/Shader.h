#pragma once

#include "core/Base.h"

enum class ShaderType {
	None = 0, Vertex, TessControl, TessEvaluate, Geometry, Fragment, Compute
};

class Shader {
public:
	Shader(std::string path, ShaderType type);
	~Shader();

	const ShaderType& GetType() { return m_Type; }
	uint32_t GetShaderID() const { return m_RendererID; }
private:
	uint32_t m_RendererID;
	ShaderType m_Type;
};
