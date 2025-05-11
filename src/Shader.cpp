#include "Shader.hpp"

const char* Shader::SHADER_PROJECTION_MATRIX_NAME = "projection";
const char* Shader::SHADER_VIEW_MATRIX_NAME = "view";
const char* Shader::SHADER_MODEL_MATRIX_NAME = "model";
const char* Shader::SHADER_COLOR_VERTEX_NAME = "vertexColor";

Shader::Shader(const char* vert, const char* frag, ShaderLoadType shaderLoadType)
{
	const char* vShaderCode;
	const char* fShaderCode;

	if (shaderLoadType == ShaderLoadType::FromFile)
	{
		vShaderCode = loadShaderSourceFromFile(vert);
		fShaderCode = loadShaderSourceFromFile(frag);
	}
	else
	{
		vShaderCode = vert;
		fShaderCode = frag;
	}


	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	checkCompileErrors(vertex, "Vertex");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	checkCompileErrors(fragment, "Fragment");

	Id = glCreateProgram();
	glAttachShader(Id, vertex);
	glAttachShader(Id, fragment);
	glLinkProgram(Id);

	checkCompileErrors(Id, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

const char* Shader::loadShaderSourceFromFile(const char* shaderPath)
{
	std::string code;
	std::ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		file.open(code);
		std::stringstream shaderStream;

		shaderStream << file.rdbuf();

		file.close();

		code = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		throw std::runtime_error("Failed to read shader from  file");
	}

	return code.c_str();
}

Shader::~Shader()
{
	glDeleteProgram(Id);
}

void Shader::connect() const
{
	glUseProgram(Id);
}

void Shader::disconnect()
{
	glUseProgram(0);
}

void Shader::setBool(const std::string& name, bool value)const
{
	glUniform1i(glGetUniformLocation(Id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value)const
{
	glUniform1i(glGetUniformLocation(Id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value)const
{
	glUniform1f(glGetUniformLocation(Id, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value)const
{
	glUniform2fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
}

void Shader::setiVec2(const std::string& name, const glm::ivec2& value)const
{
	glUniform2iv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(Id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(Id, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(Id, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			throw std::runtime_error(type + " shader compilation failed");
			std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
		}
	}
	else
	{
		glGetShaderiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			throw std::runtime_error("Shader program compilation failed");
		}
	}
}

void Shader::setProjectionMatrix(const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(Id, SHADER_PROJECTION_MATRIX_NAME), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setModelMatrix(const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(Id, SHADER_MODEL_MATRIX_NAME), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setViewMatrix(const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(Id, SHADER_VIEW_MATRIX_NAME), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setColor(const glm::vec4& vec) const
{
	glUniform4fv(glGetUniformLocation(Id, SHADER_COLOR_VERTEX_NAME), 1, &vec[0]);
}

void Shader::setSampler(const std::string samplerName, int val) const
{
	setInt(samplerName, val);
}
