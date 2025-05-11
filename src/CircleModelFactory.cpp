#include "CircleModelFactory.hpp"

CircleModleFacotry::CircleModleFacotry(unsigned int countOfPoints)
	:BaseModelFactory(), mCountOfPoints(countOfPoints)
{

}

Model* CircleModleFacotry::create()
{
	mModelRenderHandle = createHandle();

	auto model = new Model(mModelRenderHandle, posVec, rotateVec, angle, scaleVec, colorVec);

	return model;
}

Model* CircleModleFacotry::create(const glm::vec3& posVec, const glm::vec3& rotateVec, float angle, const glm::vec3& scaleVec, const glm::vec4& colorVec)
{
	mModelRenderHandle = createHandle();

	auto model = new Model(mModelRenderHandle, posVec, rotateVec, angle, scaleVec, colorVec);

	return model;
}

CircleModleFacotry::~CircleModleFacotry()
{
}

ModelRenderHandle CircleModleFacotry::createHandle()
{
	if (!isHasChache)
	{
		const float maxAngle = 360.0f;
		const int countOfTringleVertices = 3;

		float minAngle = maxAngle / (float)mCountOfPoints;

		unsigned int verticesLength = (mCountOfPoints + 1) * countOfTringleVertices;
		unsigned int indicesLenght = mCountOfPoints * countOfTringleVertices;

		float* vertices = new float[verticesLength];
		unsigned int* indices = new unsigned int[indicesLenght];

		// central point
		vertices[0] = 0.0f;
		vertices[1] = 0.0f;
		vertices[2] = 0.0f;

		for (unsigned int i = 1; i < mCountOfPoints + 1; i++)
		{
			auto currentAngle = minAngle * i;
			vertices[i * countOfTringleVertices + 0] = glm::cos(glm::radians(currentAngle));
			vertices[i * countOfTringleVertices + 1] = glm::sin(glm::radians(currentAngle));
			vertices[i * countOfTringleVertices + 2] = 0.0f;
		}

		for (unsigned int i = 0; i < mCountOfPoints; i++)
		{
			unsigned int i2;

			if (i == mCountOfPoints - 1)
			{
				i2 = 1;
			}
			else
			{
				i2 = i + 2;
			}

			indices[i * countOfTringleVertices + 0] = i + 1;
			indices[i * countOfTringleVertices + 1] = i2;
			indices[i * countOfTringleVertices + 2] = 0;
		}

		mModelRenderHandle = mOpenGLModelLoader.loadModel(vertices, verticesLength, indices, indicesLenght);

		delete[] vertices;
		delete[] indices;

		isHasChache = true;
	}

	return mModelRenderHandle;
}
