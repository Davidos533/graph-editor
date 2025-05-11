#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cursor final
{
private:
	float mGlfwWindowWidth;
	float mGlfwWindowHeigth;

	float mXMousePosition;
	float mYMousePosition;

	float mZoomValue;

	float mAdditionCameraXCoord;
	float mAdditionCameraYCoord;

	float mXNDC;
	float mYNDC;

	glm::vec2 startClickCoord;
public:
	Cursor(int glfwWindowWidth, int glfwWindowHeigth, float xMousePosition, float yMousePosition, float zoomValue, float additionCameraXCoord, float additionCameraYCoord);

	void updateWindowSizes(const glm::ivec2& windowSizes);

	void updateZoomValue(float zoomValue);

	void updateMousePosition(const glm::vec2& mousePosition);
	void updateCameraCoords(const glm::vec2& cameraPosition);

	void calculateCursorNDCCoords();
	const glm::vec2& calculateCursorNDCCoordsDifference();

	glm::vec2 getCursorPosition();
	glm::vec2 getCursorPositionWithoutCameraAndZoom();

	float getXNDCCoord();
	float getYNDCCoord();
};

#endif