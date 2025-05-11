#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include <iostream>
#include <exception>
#include <string>

#include <glad/glad.h>

#include <stb_image.h>

class TextureLoader
{
public:
	TextureLoader();

	GLuint loadTexture(const std::string pathToTexture);

	~TextureLoader();
};

#endif