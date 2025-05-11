#ifndef SQUARE_MODEL_FACTORY_HPP
#define SQUARE_MODEL_FACTORY_HPP

#include "BaseModelFactory.hpp"

class SquareModelFactory : public BaseModelFactory
{
public:
	std::string PathToTexture;
	SquareModelFactory(std::string pathToTexture = "");

	virtual Model* create() override;
	virtual Model* create(const glm::vec3& posVec, const glm::vec3& rotateVec, float angle, const glm::vec3& scaleVec, const glm::vec4& colorVec) override;

	virtual ~SquareModelFactory();
private:
	ModelRenderHandle createHandle();
};

#endif
