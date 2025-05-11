#include "Model.hpp"

Model::Model(const ModelRenderHandle& modelRenderHandle)
	:Model(modelRenderHandle, glm::vec3(0.0f), glm::vec3(0.0f,0.0f,1.0f), 0.0f, glm::vec3(1.0f,1.0f,1.0f), glm::vec4(1.0f))
{

}

Model::Model(const ModelRenderHandle& modelRenderHandle, const glm::vec3& posVec, const glm::vec3& rotateVec, float angle, const glm::vec3& scaleVec, const glm::vec4& colorVec)
: mModelRenderHandle(modelRenderHandle), mPosVec(posVec), mScaleVec(scaleVec), mAngle(angle), mRotateVec(rotateVec), mColor(colorVec), mColorChange(1.0f)
{
}

Model::~Model()
{

}

void Model::setPos(const glm::vec3& pos)
{
	mPosVec = pos;
}

void Model::setScale(const glm::vec3& scale)
{
	mScaleVec = scale;
}

void Model::setRotate(float angle, const glm::vec3& rotateVec)
{
	mAngle = angle;
	mRotateVec = rotateVec;
}

void Model::isSelect(bool isSelect)
{
	if (isSelect)
	{
		mColorChange = 0.5f;
		return;
	}

	mColorChange = 1.0f;
}

const glm::vec3& Model::getPos()
{
	return mPosVec;
}

const glm::vec3& Model::getSclae()
{
	return mScaleVec;
}

const glm::vec4& Model::getColor()
{
	return mColor;
}

const glm::vec4& Model::getRotate()
{
	return glm::vec4(mRotateVec, mAngle);
}

void Model::setColor(const glm::vec4 color)
{
	mColor = color;
}

void Model::render(const Shader& shader)
{
	shader.connect();

	mModelMatrix = glm::mat4(1.0f);
	mModelMatrix = glm::translate(mModelMatrix, mPosVec);
	mModelMatrix = glm::rotate(mModelMatrix, mAngle, mRotateVec);
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(mScaleVec));

	shader.setModelMatrix(mModelMatrix);
	shader.setColor(mColor * mColorChange);

	glBindTexture(GL_TEXTURE_2D, mModelRenderHandle.TextureId);
	glBindVertexArray(mModelRenderHandle.VAO);
	glDrawElements(GL_TRIANGLES, mModelRenderHandle.CountOfIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	shader.disconnect();
}