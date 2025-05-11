#include "Font.hpp"

const char* Font::SCREEN_FONT_SHADER_VERT =
"#version 330 core\n"
"\n"
"layout(location = 0)in vec4 vertex;\n"
"out vec2 TexCoords;\n"
"uniform mat4 projection;\n"
"\n"
"void main()\n"
"{\n"
"gl_Position = projection * vec4(vertex.xy, 0.0f, 1.0f);\n"
"TexCoords = vertex.zw;\n"
"}\n";

const char* Font::SCREEN_FONT_SHADER_FRAG =
"#version 330 core\n"
"\n"
"in vec2 TexCoords;\n"
"out vec4 color;\n"
"uniform sampler2D text; \n"
"uniform vec4 textColor;\n"
"\n"
"void main()\n"
"{\n"
"vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(text, TexCoords).r); \n"
"color = vec4(textColor) * sampled;\n"
"}\n";

const char* Font::WORLD_FONT_SHADER_VERT =
"#version 330 core\n"
"\n"
"layout(location = 0)in vec4 vertex;\n"
"out vec2 TexCoords;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"\n"
"void main()\n"
"{\n"
"gl_Position = projection * view * vec4(vertex.xy, 0.0f, 1.0f);\n"
"TexCoords = vertex.zw;\n"
"}\n";

const char* Font::WORLD_FONT_SHADER_FRAG =
"#version 330 core\n"
"\n"
"in vec2 TexCoords;\n"
"out vec4 color;\n"
"uniform sampler2D text; \n"
"uniform vec4 textColor;\n"
"\n"
"void main()\n"
"{\n"
"vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(text, TexCoords).r); \n"
"color = vec4(textColor) * sampled;\n"
"}\n";

Font::Font(const char* fontPath, FontShaderType fontShaderType)
{
	if (FT_Init_FreeType(&mFt))
	{
		throw std::runtime_error("ERROR::file_FontRenderer.hpp::type_FontRenderer::method_FontRenderer()::message_freetype library cannot initialize");
	}

	if (FT_New_Face(mFt, fontPath, 0, &mFace))
	{
		throw std::runtime_error("ERROR::file_FontRenderer.hpp::type_FontRenderer::method_FontRenderer()::message_freetype failed to load font");
	}

	FT_Set_Pixel_Sizes(mFace, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned int c = 0; c < 256; c++)
	{
		if (FT_Load_Char(mFace, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph\n";
			continue;
		}
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			mFace->glyph->bitmap.width,
			mFace->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			mFace->glyph->bitmap.buffer
		);

		Character character =
		{
			texture,
			glm::ivec2(mFace->glyph->bitmap.width,mFace->glyph->bitmap.rows),
			glm::ivec2(mFace->glyph->bitmap_left,mFace->glyph->bitmap_top),
			static_cast<unsigned int>(mFace->glyph->advance.x)

		};

		mCharacters.insert(std::pair<char, Character>(c, character));

		if (character.Size.y > mMaxCharacterHeight)
		{
			mMaxCharacterHeight = character.Size.y;
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 0);

	glGenVertexArrays(1, &mFontVAO);
	glGenBuffers(1, &mFontVBO);

	glBindVertexArray(mFontVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mFontVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	if (mShader != nullptr)
	{
		return;
	}

	if (fontShaderType == FontShaderType::WorldFont)
	{
		mShader = new Shader(WORLD_FONT_SHADER_VERT, WORLD_FONT_SHADER_FRAG, Shader::ShaderLoadType::FromString);
	}
	if (fontShaderType == FontShaderType::ScreeFont)
	{
		mShader = new Shader(SCREEN_FONT_SHADER_VERT, SCREEN_FONT_SHADER_FRAG, Shader::ShaderLoadType::FromString);
	}
}

Font::~Font()
{
	delete mShader;
}

void Font::setProjectionMatrix(const glm::mat4& projection)
{
	mShader->connect();
	mShader->setProjectionMatrix(projection);
	mShader->disconnect();
}

void Font::setViewMatrix(const glm::mat4& view)
{
	mShader->connect();
	mShader->setViewMatrix(view);
	mShader->disconnect();
}

void Font::renderText(const std::string& text, const glm::vec2& position, float scale, const glm::vec4& color)
{
	mShader->connect();
	mShader->setVec4("textColor", color);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(mFontVAO);

	float startXPos = position.x;

	auto lPosition = position;

	for (auto& letter : text)
	{
		Character ch = mCharacters[letter];

		if (letter == '\n')
		{
			lPosition.x = startXPos;
			lPosition.y -= mMaxCharacterHeight * scale;
			continue;
		}

		float xpos = lPosition.x + ch.Bearing.x * scale;
		float ypos = lPosition.y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		// coordinate triangle
		float vertices[6][4] =
		{
			// top left
			{xpos,ypos + h,0.0f,0.0f},
			// bottom left
			{xpos,ypos,0.0f,1.0f},
			//bottom right
			{xpos + w,ypos,1.0f,1.0f},

			// top left
			{xpos,ypos + h,0.0f,0.0f},
			// bottom right
			{xpos + w,ypos,1.0f,1.0f},
			// top right
			{xpos + w,ypos + h,1.0f,0.0f}
		};

		glBindTexture(GL_TEXTURE_2D, ch.textureID);

		glBindBuffer(GL_ARRAY_BUFFER, mFontVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		lPosition.x += (ch.Advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	Shader::disconnect();
}