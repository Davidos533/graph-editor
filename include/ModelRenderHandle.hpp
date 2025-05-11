#ifndef MODEL_RENDER_HANDLE
#define MODEL_RENDER_HANDLE

#include <glad/glad.h>

struct ModelRenderHandle
{
	GLuint VAO = -1;
	GLuint VBO = -1;
	GLuint EBO = -1;
	unsigned int CountOfIndices = -1;
	GLuint TextureId = -1;
};

#endif