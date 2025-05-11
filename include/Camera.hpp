#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;

	float mYaw;
	float mPitch;

	float mMovementSpeed = 1.0f;
	float mMouseSensivity = 1.0f;
	float mZoom = 1.0f;
	float mZoomSpeed = 0.1f;
public:
	enum class Direction
	{
		Forward,
		Backward,
		Left,
		Right
	};

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 90.0f, float pitch = 0.0f, float zoomSpeed = 0.1f);

	const glm::vec3& getPosition() const;

	void setPosition(const glm::vec3& position);

	void setZoom(float zoom);

	void processKeyboard(Direction direction, float deltaTime);

	glm::mat4 getViewMatrix();

	float getZoom();

	void scroll(float scrollValue);

	~Camera();
private:
	void updateCamraVectors();
};

#endif