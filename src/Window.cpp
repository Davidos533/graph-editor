#include "Window.hpp"

const char* Window::vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
"}\0";

const char* Window::fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   FragColor = vertexColor;\n"
"}\n\0";

Window::Window(const WindowConfiguration& windowConfiguration,
	const OpenGLConfiguration& openGLConfiguration)
	:RenderWindow(windowConfiguration, openGLConfiguration), mFont(nullptr), mShader(nullptr), mCursor(nullptr), mBaseModelFactory(nullptr)
{

}

Window::~Window()
{
	delete mShader;
	delete mBaseModelFactory;
	delete mCursor;
	delete mFont;
	delete mScene;
	delete mSceneFont;
}

void Window::onLoad()
{
	mShader = new Shader(vertexShaderSource, fragmentShaderSource, Shader::ShaderLoadType::FromString);
	mScreenShader = new Shader(vertexShaderSource, fragmentShaderSource, Shader::ShaderLoadType::FromString);
	mCursor = new Cursor(mWindowConfiguration.Width, mWindowConfiguration.Height, 0,0, mCamera->getZoom(), mCamera->getPosition().x, mCamera->getPosition().y);
	mFont = new Font("MysteryQuest-Regular.ttf");
	mSceneFont = new Font("MysteryQuest-Regular.ttf", FontShaderType::WorldFont);
	mScene = new ProgramScene(*mCursor, *mSceneFont);

	mShader->connect();
	mShader->setProjectionMatrix(mProjection->getProjectionOrthographicMatrix());

	mScreenShader->connect();
	mScreenShader->setProjectionMatrix(mProjection->getProjectionOrthographicMatrix());
	mScreenShader->setViewMatrix(mCamera->getViewMatrix());

	mFont->setProjectionMatrix(mProjection->getProjectionOrthographicMatrix());
	mSceneFont->setProjectionMatrix(mProjection->getProjectionOrthographicMatrix());

	mBaseModelFactory = new SquareModelFactory();

	mDijkstraButton = mBaseModelFactory->create(glm::vec3(0.87f,0.7f, 0.0f),glm::vec3(0.0f,0.0f,1.0f),0.0f, glm::vec3(0.4f,0.2f,0.0f),glm::vec4(0.7f, 0.7f, 0.0f, 1.0f));
	mKruskalButton = mBaseModelFactory->create(glm::vec3(0.87f, 0.4f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, glm::vec3(0.4f, 0.2f, 0.0f), glm::vec4(0.7f, 0.7f, 0.0f, 1.0f));
	mResetColorButton = mBaseModelFactory->create(glm::vec3(0.87f, 0.1f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, glm::vec3(0.4f, 0.2f, 0.0f), glm::vec4(0.7f, 0.7f, 0.0f, 1.0f));
}

void Window::onCursorMoove(double xPos, double yPos)
{
	mCursor->updateCameraCoords(mCamera->getPosition());
	mCursor->updateZoomValue(mCamera->getZoom());
	mCursor->updateMousePosition(glm::vec2(xPos, yPos));

	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		auto diffCorrds = mCursor->calculateCursorNDCCoordsDifference();
		auto camPos = mCamera->getPosition();

		mCamera->setPosition(camPos - glm::vec3(diffCorrds,0.0f));
	}

	mCursor->calculateCursorNDCCoords();

	mScene->onCursorMoove(glm::vec2(xPos, yPos), glm::vec2(mCursor->getXNDCCoord(), mCursor->getYNDCCoord()));

	mDijkstraButton->isSelect(checkSquareColide(mDijkstraButton->getPos(), mDijkstraButton->getSclae(), mCursor->getCursorPositionWithoutCameraAndZoom()));
	mKruskalButton->isSelect(checkSquareColide(mKruskalButton->getPos(), mKruskalButton->getSclae(), mCursor->getCursorPositionWithoutCameraAndZoom()));
	mResetColorButton->isSelect(checkSquareColide(mResetColorButton->getPos(), mResetColorButton->getSclae(), mCursor->getCursorPositionWithoutCameraAndZoom()));
}

void Window::onMouseClick(int button, int action, int mods)
{
	RenderWindow::onMouseClick(button, action, mods);
}

void Window::onMouseClickLeft()
{
	if (checkSquareColide(mDijkstraButton->getPos(), mDijkstraButton->getSclae(), mCursor->getCursorPositionWithoutCameraAndZoom()))
	{
		if(mScene->isCompletedGraph())
		{
			mScene->startDijkstra();
		}
	}
	else if (checkSquareColide(mKruskalButton->getPos(), mKruskalButton->getSclae(), mCursor->getCursorPositionWithoutCameraAndZoom()))
	{
		if(mScene->isCompletedGraph())
		{
			mScene->startKruskal();
		}
	}
	else if(checkSquareColide(mResetColorButton->getPos(), mResetColorButton->getSclae(), mCursor->getCursorPositionWithoutCameraAndZoom()))
	{
		mScene->resetArcsColors();
	}
	else
	{
		mScene->onClickLeft(mCursor->getCursorPosition().x, mCursor->getCursorPosition().y);
	}
}

void Window::onMouseClickRight()
{
	mScene->onClickRight(mCursor->getCursorPosition().x, mCursor->getCursorPosition().y);
}

void Window::onScroll(double xOffset, double yOffset)
{
	// camera
	mCamera->scroll(yOffset);

	// projection
	mProjection->updateProjectionMatrices(mWindowConfiguration.Width, mWindowConfiguration.Height, mCamera->getZoom());

	// another projection updates
	auto projectionOrthoMatrix = mProjection->getProjectionOrthographicMatrix();

	mShader->connect();

	mShader->setProjectionMatrix(projectionOrthoMatrix);
	mSceneFont->setProjectionMatrix(projectionOrthoMatrix);
	mShader->disconnect();
}

void Window::onTyping(GLFWwindow* window, unsigned int codepoint)
{
	auto text = UnicodeToUTF8(codepoint);
	if (text.size() == 1)
	{
		mScene->onTyping(text);
	}
}

void Window::onUpdate()
{
	// update view matrix
	auto viewMatrix = mCamera->getViewMatrix();

	mShader->connect();
	mShader->setViewMatrix(viewMatrix);
	mSceneFont->setViewMatrix(viewMatrix);
	mShader->disconnect();

	mScene->onUpdate();
}

void Window::onRender()
{
	mScene->onRender(*mShader);

	auto& shader = *mScreenShader;

	mDijkstraButton->render(shader);

	mKruskalButton->render(shader);
	
	mResetColorButton->render(shader);

	mFont->renderText("Cur world pos:\n" + std::to_string(mCursor->getXNDCCoord()) + "\n" + std::to_string(mCursor->getYNDCCoord())+"\nCamera pos:\n"+std::to_string(mCamera->getPosition().x)+"\n"+ std::to_string(mCamera->getPosition().y), glm::vec2(-1.3f, 0.9f), 0.0015f);
	mFont->renderText("Dijkstra alg", glm::vec2(0.7f, 0.7f), 0.0015f);
	mFont->renderText("Kruskal alg", glm::vec2(0.7f, 0.4f), 0.0015f);
	mFont->renderText("Reset color", glm::vec2(0.7f, 0.1f), 0.0015f);
}

void Window::onInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
		case GLFW_PRESS:
		{
			switch (key)
			{
				case GLFW_KEY_BACKSPACE:
				{
					mScene->onTyping("backspace");
				}break;
				case GLFW_KEY_L:
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}break;
				case GLFW_KEY_F:
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}break;
				default:
				{
					mScene->onInput(key);
				}break;
			}
		}break;
		case GLFW_REPEAT:
		{
			switch (key)
			{
				case GLFW_KEY_BACKSPACE:
				{
					mScene->onTyping("backspace");
				}break;
			}
		}
	}
}

bool Window::checkSquareColide(const glm::vec2& pos, const glm::vec2& scale, const glm::vec2& cursorPos)
{
	auto leftBorder = pos.x - scale.x / 2;
	auto rightBorder = pos.x + scale.x / 2;
	auto topBorder = pos.y + scale.y / 2;
	auto bottomBorder = pos.y - scale.y / 2;
	
	if (cursorPos.x > leftBorder
		&& cursorPos.x < rightBorder
		&& cursorPos.y > bottomBorder
		&& cursorPos.y < topBorder)
	{
		return true;
	}

	return false;
}