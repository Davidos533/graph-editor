#include "SquareModelFactory.hpp"

SquareModelFactory::SquareModelFactory(std::string pathToTexture)
	:BaseModelFactory(), PathToTexture(pathToTexture)
{

}

Model* SquareModelFactory::create()
{
	mModelRenderHandle = createHandle();

	auto model = new Model(mModelRenderHandle, posVec, rotateVec, angle, scaleVec, colorVec);

	return model;
}

Model* SquareModelFactory::create(const glm::vec3& posVec, const glm::vec3& rotateVec, float angle, const glm::vec3& scaleVec, const glm::vec4& colorVec)
{
	mModelRenderHandle = createHandle();

	return new Model(mModelRenderHandle, posVec, rotateVec, angle, scaleVec, colorVec);
}

SquareModelFactory::~SquareModelFactory()
{

}

ModelRenderHandle SquareModelFactory::createHandle()
{
	if (!isHasChache)
	{
		unsigned int vertSize;
		float *vertices;

		unsigned int indices[] =
		{
			0,1,2,
			2,3,0
		};

		if (PathToTexture == "")
		{
			vertSize = 12;
			vertices = new float[vertSize] {
				// 0 left down
				-0.5f, -0.5f, 0.0f,
				// 1 left up
				-0.5f,0.5f,0.0f,
				// 2 right up
				0.5f,0.5f,0.0f,
				//3 right down
				0.5f, -0.5f,0.0f
			};
		}
		else
		{
			vertSize = 20;
			vertices = new float[vertSize] {
				// 0 left down
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 
				// 1 left up
				-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
				// 2 right up
				0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
				//3 right down
				0.5f, -0.5f, 0.0f, 1.0f, 0.0f
			};
		}

		auto temp = mOpenGLModelLoader.loadModel(vertices, vertSize, indices, sizeof(indices), PathToTexture);
		mModelRenderHandle = temp;
		isHasChache = true;

		delete vertices;
	}

	return mModelRenderHandle;
}
