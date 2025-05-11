#ifndef MY_FONT_HPP
#define MY_FONT_HPP

#include <map>
#include <string>
#include <iostream>

#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

enum class FontShaderType
{
	WorldFont,
	ScreeFont
};

// class to work with freetype library and different font extensions
class Font 
{
private:
	// struc whose contains one character from font file
	struct Character
	{
		unsigned int textureID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		unsigned int Advance;
	};

	// map with values char - font character
	std::map<char, Character> mCharacters;
	float mMaxCharacterHeight = 0.0f;

	FT_Library mFt;
	FT_Face mFace;

	unsigned int mFontVAO;
	unsigned int mFontVBO;

	Shader* mShader = nullptr;

	static const char* SCREEN_FONT_SHADER_VERT;
	static const char* WORLD_FONT_SHADER_VERT;

	static const char* SCREEN_FONT_SHADER_FRAG;
	static const char* WORLD_FONT_SHADER_FRAG;
public:
	Font(const char* fontPath, FontShaderType fontShaderType = FontShaderType::ScreeFont);

	void renderText(const std::string& text,const glm::vec2& position, float scale = 0.005f, const glm::vec4& color = glm::vec4(1.0f));

	void setProjectionMatrix(const glm::mat4& projection);
	void setViewMatrix(const glm::mat4& view);

	~Font();
};

#endif