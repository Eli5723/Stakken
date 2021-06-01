#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader {
	public:
		unsigned int ID;

		Shader(const std::string& path);
        Shader(const char* fShaderCode, const char* vShaderCode);

		void use();
		void setBool(const char* name, bool value);
		void setInt(const char* name, int value);
		void setFloat(const char* name, float value);
		void setMat4(const char* name, const glm::mat4& );
		void setMat4Offset(const char* name, const glm::mat4 &value, int offset);
		void setVec2(const char* name, float* value);
		void fillSamplers();
};