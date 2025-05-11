#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include <stdexcept>
#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "WindowConfiguration.hpp"
#include "OpenGLConfiguration.hpp"
#include "SquareModelFactory.hpp"

#include "Shader.hpp"

#include "Camera.hpp"
#include "Projection.hpp"

class RenderWindow
{
protected:
	static const char* vertexShaderSource;
	static const char* fragmentShaderSource;

	OpenGLConfiguration mOpenGLConfiguration;
	WindowConfiguration mWindowConfiguration;
	
	GLFWwindow* mWindow = nullptr;

	Projection* mProjection;
	Camera* mCamera;

	float mDeltaTime = 0.0f;
	float mLastFrame = 0.0f;

	double mMousePressRepeatDelay = 0.15f;
	double mLastMousePressTime = 0.0f;
	bool mIsMousePressed = true;

	SquareModelFactory mSquareModelFactory;

	Model* mSplashScreen;
	Shader *mSplashScreeShader;

	float mSplashScreenTime = 3.5f;
	float mSplashHideDelay = 0.0f;
public:
	RenderWindow(const RenderWindow& obj) = delete;

	RenderWindow(const WindowConfiguration& windowConfiguration,
		const OpenGLConfiguration& openGLConfiguration);

	virtual void start() final;

	virtual void onCursorMoove(double xPos, double yPos);
	virtual void onMouseClick(int button, int action, int mods);
	virtual void onMouseClickLeft();
	virtual void onMouseClickRight();
	virtual void onScroll(double xOffset, double yOffset);
	virtual void onInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	virtual void onTyping(GLFWwindow* window, unsigned int codepoint);

	virtual ~RenderWindow();
protected:
	virtual void onLoad();

	// rendering
	virtual void onRender();

	virtual void onUpdate();

	// functional
	void setBakcgroundColor(float red, float green, float blue, float alpha);

	double getTimeFromStart();
	std::string UnicodeToUTF8(unsigned int codepoint);
private:
	void initialize();

	void afterRender();

	void beforeRender();

	void initializeGLFW();
	void initializeGLFWCallbacks();

	void initializeGLAD();

	void settingUpOpenGL();

};
#endif