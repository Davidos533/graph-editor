#include "Cursor.hpp"

Cursor::Cursor(int glfwWindowWidth, int glfwWindowHeigth, float xMousePosition, float yMousePosition, float additionCameraXCoord, float additionCameraYCoord, float zoomValue)
	:
	mGlfwWindowWidth(glfwWindowWidth),
	mGlfwWindowHeigth(glfwWindowHeigth),
	mXMousePosition(xMousePosition),
	mYMousePosition(yMousePosition),
	mAdditionCameraXCoord(additionCameraXCoord),
	mAdditionCameraYCoord(additionCameraYCoord),
	mZoomValue(zoomValue)
{
}
void Cursor::updateWindowSizes(const glm::ivec2& windowSizes)
{
	mGlfwWindowWidth = windowSizes.x;
	mGlfwWindowHeigth = windowSizes.y;
}

void Cursor::calculateCursorNDCCoords()
{
	float  denumX = (mGlfwWindowWidth / 2);
	float  denumY = (mGlfwWindowHeigth / 2);
	float  aspect = ((float)mGlfwWindowWidth / (float)mGlfwWindowHeigth);
	
	mXNDC = (-((denumX - mXMousePosition) / denumX) * aspect * mZoomValue);
	mYNDC = (((denumY - mYMousePosition) / denumY)) * mZoomValue;

}

const glm::vec2& Cursor::calculateCursorNDCCoordsDifference()
{
	float  denumX = (mGlfwWindowWidth / 2);
	float  denumY = (mGlfwWindowHeigth / 2);
	float  aspect = ((float)mGlfwWindowWidth / (float)mGlfwWindowHeigth);

	float x = (-((denumX - mXMousePosition) / denumX) * aspect * mZoomValue);
	float y = (((denumY - mYMousePosition) / denumY)) * mZoomValue;

	return glm::vec2(x - mXNDC , y - mYNDC);
}

glm::vec2 Cursor::getCursorPosition()
{
	return glm::vec2(mXNDC + mAdditionCameraXCoord, mYNDC + mAdditionCameraYCoord);
}

glm::vec2 Cursor::getCursorPositionWithoutCameraAndZoom()
{
	float  denumX = (mGlfwWindowWidth / 2);
	float  denumY = (mGlfwWindowHeigth / 2);
	float  aspect = ((float)mGlfwWindowWidth / (float)mGlfwWindowHeigth);

	auto x = (-((denumX - mXMousePosition) / denumX) * aspect);
	auto y = (((denumY - mYMousePosition) / denumY));

	return glm::vec2(x, y);
}

void Cursor::updateZoomValue(float zoomValue)
{
	mZoomValue = zoomValue;
}
void Cursor::updateMousePosition(const glm::vec2& mousePosition)
{
	mXMousePosition = mousePosition.x;
	mYMousePosition = mousePosition.y;
}
void Cursor::updateCameraCoords(const glm::vec2& cameraPosition)
{
	mAdditionCameraXCoord = cameraPosition.x;
	mAdditionCameraYCoord = cameraPosition.y;
}
float Cursor::getXNDCCoord()
{
	return mXNDC + mAdditionCameraXCoord;
}
float Cursor::getYNDCCoord()
{
	return mYNDC + mAdditionCameraYCoord;
}
