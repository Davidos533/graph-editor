#include "RenderWindow.hpp"

const char* RenderWindow::vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos * 1.5f, 1.0f);\n"
"   TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
"}\0";

const char* RenderWindow::fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture0;\n"
"uniform float destroity = 0.0f;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(texture(texture0, TexCoord).xyz,destroity);\n"
"}\n\0";

RenderWindow::RenderWindow(const WindowConfiguration& windowConfiguration, const OpenGLConfiguration& openGLConfiguration)
	: mWindowConfiguration(windowConfiguration), mOpenGLConfiguration(openGLConfiguration)
{
	mProjection = new Projection();
	mCamera = new Camera();

	initialize();

	mSplashScreeShader = new Shader(vertexShaderSource, fragmentShaderSource, Shader::ShaderLoadType::FromString);
	mSplashScreeShader->connect();
	mSplashScreeShader->setSampler("texture0", 0);

	mSquareModelFactory.PathToTexture = "logo.png";
	mSplashScreen = mSquareModelFactory.create();
	mSplashScreeShader->connect();
	mSplashScreeShader->setFloat("destroity", 0.03f);
}

RenderWindow::~RenderWindow()
{
	glfwTerminate();

	delete mCamera;
	delete mProjection;
}

void RenderWindow::onLoad()
{

}

void RenderWindow::start()
{
	onLoad();

	while (!glfwWindowShouldClose(mWindow) && getTimeFromStart() < (mSplashHideDelay + mSplashScreenTime))
	{
		mSplashScreen->render(*mSplashScreeShader);
		afterRender();
	}

	while (!glfwWindowShouldClose(mWindow))
	{
		float currentFrame = getTimeFromStart();
		mDeltaTime = currentFrame - mLastFrame;
		mLastFrame = currentFrame;

		onUpdate();
		beforeRender();
		onRender();

		afterRender();
	}
};

void RenderWindow::initialize()
{
	initializeGLFW();
	initializeGLAD();
	settingUpOpenGL();

	mProjection->updateProjectionOrthographicMatrix(mWindowConfiguration.Width, mWindowConfiguration.Height, mCamera->getZoom());
	mProjection->updateProjectionPerspectiveMatrix(mWindowConfiguration.Width, mWindowConfiguration.Height, mCamera->getZoom());
}


void RenderWindow::initializeGLFW()
{
	auto initResult = glfwInit();

	if (initResult < 0)
	{
		throw std::runtime_error("Failed to initialize GLFW.");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, mOpenGLConfiguration.MajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, mOpenGLConfiguration.MinorVersion);

	glfwWindowHint(GLFW_OPENGL_PROFILE, mOpenGLConfiguration.UserOpenGLProfile == OpenGLProfile::Core ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, mOpenGLConfiguration.NumberOfSamples);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	mWindow = glfwCreateWindow
	(mWindowConfiguration.Width,
		mWindowConfiguration.Height,
		mWindowConfiguration.Title.c_str(),
		nullptr,
		nullptr);

	initializeGLFWCallbacks();

	if (mWindow == nullptr)
	{
		glfwTerminate();
		throw std::runtime_error("Window creation failed.");
	}

	glfwMakeContextCurrent(mWindow);
}

void RenderWindow::initializeGLFWCallbacks()
{
	glfwSetWindowUserPointer(mWindow, this);

	auto cursorMooveFunc = [](GLFWwindow* w, double xPos, double yPos)
	{
		static_cast<RenderWindow*>((glfwGetWindowUserPointer(w)))->onCursorMoove(xPos, yPos);
	};

	auto mouseClickFunc = [](GLFWwindow* w, int button, int action, int mode)
	{
		static_cast<RenderWindow*>((glfwGetWindowUserPointer(w)))->onMouseClick(button, action, mode);
	};

	auto scrollFunc = [](GLFWwindow* w, double xOffset, double yOffset)
	{
		static_cast<RenderWindow*>((glfwGetWindowUserPointer(w)))->onScroll(xOffset, yOffset);
	};

	auto keyFunc = [](GLFWwindow* w, int key, int scancode, int action, int mods)
	{
		static_cast<RenderWindow*>((glfwGetWindowUserPointer(w)))->onInput(w, key, scancode, action, mods);
	};

	auto charFunc = [](GLFWwindow* w, unsigned int codepoint)
	{
		static_cast<RenderWindow*>((glfwGetWindowUserPointer(w)))->onTyping(w, codepoint);
	};

	glfwSetCursorPosCallback(mWindow, cursorMooveFunc);
	glfwSetMouseButtonCallback(mWindow, mouseClickFunc);
	glfwSetScrollCallback(mWindow, scrollFunc);
	glfwSetKeyCallback(mWindow, keyFunc);
	glfwSetCharCallback(mWindow, charFunc);
}

void RenderWindow::initializeGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("OpenGL context with window connection failed.");
	}
}

void RenderWindow::settingUpOpenGL()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	//glViewport(0, 0, mWindowConfiguration.Width, mWindowConfiguration.Height);
	glViewport(0, 0, mWindowConfiguration.Width, mWindowConfiguration.Height);
}

std::string RenderWindow::UnicodeToUTF8(unsigned int codepoint)
{
	std::string out;

	if (codepoint <= 0x7f)
		out.append(1, static_cast<char>(codepoint));
	else if (codepoint <= 0x7ff)
	{
		out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
		out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
	}
	else if (codepoint <= 0xffff)
	{
		out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
		out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
		out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
	}
	else
	{
		out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
		out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
		out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
		out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
	}
	return out;
}

void RenderWindow::setBakcgroundColor(float red, float green, float blue, float alpha)
{
	mOpenGLConfiguration.BackgroundFill.Red = red;
	mOpenGLConfiguration.BackgroundFill.Green = green;
	mOpenGLConfiguration.BackgroundFill.Blue = blue;
	mOpenGLConfiguration.BackgroundFill.Alpha = alpha;
}

double RenderWindow::getTimeFromStart()
{
	return glfwGetTime();
}

void RenderWindow::onRender()
{

}

void RenderWindow::afterRender()
{
	glfwSwapInterval(1);
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

void RenderWindow::beforeRender()
{
	glClearColor
	(mOpenGLConfiguration.BackgroundFill.Red,
		mOpenGLConfiguration.BackgroundFill.Green,
		mOpenGLConfiguration.BackgroundFill.Blue,
		mOpenGLConfiguration.BackgroundFill.Alpha);

	glClear(GL_COLOR_BUFFER_BIT);
}


void RenderWindow::onUpdate()
{

}

void RenderWindow::onInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

void RenderWindow::onTyping(GLFWwindow* window, unsigned int codepoint)
{

}

void RenderWindow::onCursorMoove(double xPos, double yPos)
{

}

void RenderWindow::onMouseClick(int button, int action, int mods)
{
	mIsMousePressed = false;

	switch (action)
	{
		case GLFW_PRESS:
		{
			mIsMousePressed = true;
			mLastMousePressTime = getTimeFromStart();

			if (button == GLFW_MOUSE_BUTTON_LEFT)
			{
				onMouseClickLeft();
			}
		};
	}

	if (!mIsMousePressed && (getTimeFromStart() - mLastMousePressTime) < mMousePressRepeatDelay)
	{
		mIsMousePressed = true;
		switch (button)
		{
			case GLFW_MOUSE_BUTTON_RIGHT:
			{
				onMouseClickRight();
			}break;
		}
	}
}

void RenderWindow::onMouseClickLeft()
{

}

void RenderWindow::onMouseClickRight()
{

}

void RenderWindow::onScroll(double xOffset, double yOffset)
{

}
