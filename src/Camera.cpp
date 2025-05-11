#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float zoomSpeed)
	:mPosition(position), mUp(up), mYaw(yaw), mPitch(pitch), mZoomSpeed(zoomSpeed)
{
	updateCamraVectors();
}

const glm::vec3& Camera::getPosition() const
{
	return mPosition;
}

void Camera::processKeyboard(Direction direction, float deltaTime)
{
	float velocity = mMovementSpeed * deltaTime;

	switch (direction)
	{
		case Direction::Forward:
		{
			mPosition.z -= velocity;
		}break;
		case Direction::Backward:
		{
			mPosition.z += velocity;
		}break;
		case Direction::Left:
		{
			mPosition.x -= velocity;
		}break;
		case Direction::Right:
		{
			mPosition.x += velocity;
		}break;
	}
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::updateCamraVectors()
{
	glm::vec3 front;

	mFront = glm::vec3(0.0f, 0.0f, -1.0f);
	mUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::setPosition(const glm::vec3& position)
{
	mPosition = position;
}

void Camera::setZoom(float zoom)
{
	mZoom = zoom;
}

float Camera::getZoom()
{
	return mZoom;
}

void Camera::scroll(float scrollValue)
{
	bool isZoomShouldStop = (mZoom + -scrollValue * mZoomSpeed) <= mZoomSpeed*2;
	if (isZoomShouldStop)
	{
		if (scrollValue < 0)
		{
			mZoom += -scrollValue * mZoomSpeed;
		}
		return;
	}

	mZoom += -scrollValue * mZoomSpeed;
}

Camera::~Camera()
{

}