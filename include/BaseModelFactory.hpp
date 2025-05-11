#ifndef MODEL_FACTORY_HPP
#define MODEL_FACTORY_HPP

#include "OpenGLModelLoader.hpp"
#include "Model.hpp"
#include "Shader.hpp"

class BaseModelFactory
{
protected:
	explicit BaseModelFactory();
	OpenGLModelLoader mOpenGLModelLoader;
	ModelRenderHandle mModelRenderHandle;
	bool isHasChache = false;
public:
	glm::vec3 posVec = glm::vec3(0.0f);
	glm::vec3 rotateVec = glm::vec3(0.0f,0.0f,1.0f);
	float angle = 0.0f;
	glm::vec3 scaleVec = glm::vec3(1.0f);
	glm::vec4 colorVec = glm::vec4(1.0f);

	virtual Model* create() = 0;
	virtual Model* create(const glm::vec3& posVec, const glm::vec3& rotateVec, float angle, const glm::vec3& scaleVec, const glm::vec4& colorVec) = 0;

	virtual ~BaseModelFactory();
};

#endif
