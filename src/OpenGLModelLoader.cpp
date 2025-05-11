#include "OpenGLModelLoader.hpp"

const ModelRenderHandle OpenGLModelLoader::loadModel(float* vertices, unsigned int verticesLenght, unsigned int* indices, unsigned int indicesLenght, std::string pathToTexture)
{
	GLuint VAO, VBO, EBO;

	bool isTextureHas = pathToTexture != "";

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesLenght, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesLenght, indices, GL_STATIC_DRAW);

	if (isTextureHas)
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	else
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	ModelRenderHandle ren;
	
	ren.VAO = VAO;
	ren.VBO = VBO;
	ren.EBO = EBO;
	ren.CountOfIndices = indicesLenght;

	if (isTextureHas)
	{
		ren.TextureId = mTextureLoader.loadTexture(pathToTexture);
	}

	return ren;
}
