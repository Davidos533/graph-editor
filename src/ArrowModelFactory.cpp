#include "ArrowModelFactory.hpp"

ArrowModelFactory::ArrowModelFactory()
    :BaseModelFactory()
{

}

Model* ArrowModelFactory::create()
{
	mModelRenderHandle = createHandle();

	auto model = new Model(mModelRenderHandle, posVec, rotateVec, angle, scaleVec, colorVec);

	return model;
}

Model* ArrowModelFactory::create(const glm::vec3& posVec, const glm::vec3& rotateVec, float angle, const glm::vec3& scaleVec, const glm::vec4& colorVec)
{
	mModelRenderHandle = createHandle();

	return new Model(mModelRenderHandle, posVec, rotateVec, angle, scaleVec, colorVec);
}

ArrowModelFactory::~ArrowModelFactory()
{

}

ModelRenderHandle ArrowModelFactory::createHandle()
{
	if (!isHasChache)
	{
		float vertices[] =
		{
			// 0 left down
			-0.1f, -0.5f, 0.0f,
			// 1 left up
			-0.1f,0.4f,0.0f,
			// 2 right up
			0.1f,0.4f,0.0f,
			//3 right down
			0.1f, -0.5f,0.0f,
			// 4 left triangle
			-0.3f, 0.35f,0.0f,
			// 5 right triangle
			0.3f,0.35f,0.0f,
			// 6 top triangle
			0.0f, 0.5f,0.0f,
		};

		unsigned int indices[] =
		{
			0,1,2,
			2,3,0,
			4,5,6
		};

		mModelRenderHandle = mOpenGLModelLoader.loadModel(vertices, sizeof(vertices), indices, sizeof(indices));
		isHasChache = true;
	}

	return mModelRenderHandle;
}
