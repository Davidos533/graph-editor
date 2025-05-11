#ifndef MODEL_HPP
#define MODEL_HPP

#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "ModelRenderHandle.hpp"

class Model
{
private:
	ModelRenderHandle mModelRenderHandle;
	glm::mat4 mModelMatrix;
	glm::vec3 mPosVec;
	glm::vec3 mRotateVec;
	float mAngle;
	glm::vec3 mScaleVec;
	glm::vec4 mColor;
	float mColorChange;
public:
	Model(const ModelRenderHandle& modelRenderHandle);
	Model(const ModelRenderHandle& modelRenderHandle, const glm::vec3& posVec, const glm::vec3& rotateVec, float angle, const glm::vec3& scaleVec, const glm::vec4& colorVec);

	void setPos(const glm::vec3& pos);
	void setScale(const glm::vec3& scale);
	void setColor(const glm::vec4 color);
	void setRotate(float angle, const glm::vec3& rotateVec);
	void isSelect(bool isSelect);

	const glm::vec3& getPos();
	const glm::vec3& getSclae();
	const glm::vec4& getColor();
	const glm::vec4& getRotate();

	void render(const Shader& shader);

	~Model();
};

#endif