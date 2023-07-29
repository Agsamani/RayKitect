#pragma once
#include "Shader.h"
#include <glm/glm.hpp>

// TODO : this will change later
	class Program {
	public:
		Program();
		~Program();

		void AttachShader(const Shader& shader) const;
		void LinkProgram() const;
		void UseProgram() const;


		void UploadUniformInt(const std::string& name, int value) const;
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count) const;

		void UploadUniformFloat(const std::string& name, float value) const;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;

		void UploadUniformMat2(const std::string& name, const glm::mat2& matrix) const;
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;
	private:
		uint32_t m_RendererID;
	};
