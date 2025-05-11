#ifndef CIRCLE_MODEL_FACTORY
#define CIRCLE_MODEL_FACTORY

#include "BaseModelFactory.hpp"

class CircleModleFacotry : public BaseModelFactory
{
private:
	unsigned int mCountOfPoints;
public:
	CircleModleFacotry(unsigned int countOfPoints = 10);
	
	virtual Model* create() override;
	virtual Model* create(const glm::vec3& posVec, const glm::vec3& rotateVec, float angle, const glm::vec3& scaleVec, const glm::vec4& colorVec) override;

	virtual ~CircleModleFacotry();
private:
	ModelRenderHandle createHandle();
};

#endif