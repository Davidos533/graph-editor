#include "BaseModelFactory.hpp"

BaseModelFactory::BaseModelFactory()
{

}

BaseModelFactory::~BaseModelFactory()
{
	glDeleteVertexArrays(1, &mModelRenderHandle.VAO);
	glDeleteBuffers(1, &mModelRenderHandle.VBO);
	glDeleteBuffers(1, &mModelRenderHandle.EBO);
}
