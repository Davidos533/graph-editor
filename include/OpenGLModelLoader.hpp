#ifndef OPENGL_MODEL_LOADER
#define OPENGL_MODEL_LOADER

#include <vector>
#include <string>

#include <glad/glad.h>

#include "ModelRenderHandle.hpp"
#include "TextureLoader.hpp"

class OpenGLModelLoader
{
private:
	TextureLoader mTextureLoader;
public:
	/// <summary>
	/// load in OpenGL new model by vertices and indices
	/// </summary>
	/// <param name="vertices"></param>
	/// <param name="indices"></param>
	/// <returns>VAO</returns>
	const ModelRenderHandle loadModel(float* vertices, unsigned int verticesLenght, unsigned int* indices, unsigned int indicesLenght, std::string pathToTexture = "");
};

#endif OPENGL_MODEL_LOADER