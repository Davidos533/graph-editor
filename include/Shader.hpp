#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader final
{
private:
	static const char* SHADER_PROJECTION_MATRIX_NAME;
	static const char* SHADER_VIEW_MATRIX_NAME;
	static const char* SHADER_MODEL_MATRIX_NAME;
	static const char* SHADER_COLOR_VERTEX_NAME;

public:
	enum class ShaderLoadType
	{
		FromFile,
		FromString
	};

	Shader(const char* vert, const char* frag, ShaderLoadType shaderLoadType);

	unsigned int Id;

	void connect() const;
	static void disconnect();

	void setBool(const std::string& name, bool value)const;
	void setInt(const std::string& name, int value)const;
	void setFloat(const std::string& name, float value)const;
	void setVec2(const std::string& name, const glm::vec2& value)const;
	void setVec2(const std::string& name, float x, float y) const;
	void setiVec2(const std::string& name, const glm::ivec2& value)const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

	void setProjectionMatrix(const glm::mat4& mat) const;
	void setModelMatrix(const glm::mat4& mat) const;
	void setViewMatrix(const glm::mat4& mat) const;
	void setColor(const glm::vec4& vec) const;
	void setSampler(const std::string samplerName ,int val) const;

	~Shader();
private:
	void checkCompileErrors(GLuint shader, std::string type);
	const char* loadShaderSourceFromFile(const char* shaderPath);
};

#endif