#include "Projection.hpp"

Projection::Projection()
{

}

const glm::mat4& Projection::getProjectionPerspectiveMatrix() const
{
	return mProjectionPerspectiveMatrix;
}

const glm::mat4& Projection::getProjectionOrthographicMatrix() const
{
	return mProjectionOrthographicMatrix;
}

void Projection::updateProjectionPerspectiveMatrix(int windowWidth, int windowHeight, float cameraZoom)
{
	float windowRatio = (float)windowWidth / (float)windowHeight;
	mProjectionPerspectiveMatrix = glm::perspective(glm::radians(cameraZoom), windowRatio, 0.1f, 100.0f);
}

void Projection::updateProjectionOrthographicMatrix(int windowWidth, int windowHeight, float cameraZoom)
{
	float windowRatio = (float)windowWidth / (float)windowHeight;
	mProjectionOrthographicMatrix = glm::ortho(-windowRatio * cameraZoom, windowRatio * cameraZoom, -cameraZoom, cameraZoom, 0.0f, 1.0f);
}

void Projection::updateProjectionMatrices(int windowWidth, int windowHeight, float cameraZoom)
{
	this->updateProjectionOrthographicMatrix(windowWidth, windowHeight, cameraZoom);
	this->updateProjectionPerspectiveMatrix(windowWidth, windowHeight, cameraZoom);
}

Projection::~Projection()
{

}