#ifndef ARROW_MODEL_FACTORY_HPP
#define ARROW_MODEL_FACTORY_HPP

#include "BaseModelFactory.hpp"

class ArrowModelFactory : public BaseModelFactory
{
public:
	ArrowModelFactory();
	
	virtual Model* create() override;
	virtual Model* create(const glm::vec3& posVec, const glm::vec3& rotateVec, float angle, const glm::vec3& scaleVec, const glm::vec4& colorVec) override;

	virtual ~ArrowModelFactory();
private:
	ModelRenderHandle createHandle();
};

#endif
