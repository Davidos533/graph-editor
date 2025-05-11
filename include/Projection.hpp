#ifndef PROJECTION_HPP
#define PROJECTION_HPP

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Projection
{
private:
	glm::mat4 mProjectionPerspectiveMatrix;
	glm::mat4 mProjectionOrthographicMatrix;

public:
	Projection();

	const glm::mat4& getProjectionPerspectiveMatrix() const;

	const glm::mat4& getProjectionOrthographicMatrix() const;

	void updateProjectionPerspectiveMatrix(int windowWidth, int windowHeight, float cameraZoom);

	void updateProjectionOrthographicMatrix(int windowWidth, int windowHeight, float cameraZoom);

	void updateProjectionMatrices(int windowWidth, int windowHeight, float cameraZoom);

	~Projection();
};

#endif