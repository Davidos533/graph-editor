#include "TextureLoader.hpp"

TextureLoader::TextureLoader()
{
}

GLuint TextureLoader::loadTexture(const std::string pathToTexture)
{
	GLuint textureId;

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int textureWidth;
	int textureHeigth;
	int numberOfChannels;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* textureData = stbi_load(pathToTexture.c_str(), &textureWidth, &textureHeigth, &numberOfChannels, STBI_rgb_alpha);

	if (textureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		stbi_image_free(textureData);

		throw std::runtime_error("cannot load texture data from file by this path: " + std::string(pathToTexture));
	}

	stbi_image_free(textureData);

	return textureId;
}

TextureLoader::~TextureLoader()
{
}
