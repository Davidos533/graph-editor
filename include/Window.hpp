#ifndef  WINDOW
#define WINDOW

#include "RenderWindow.hpp"
#include "OpenGLModelLoader.hpp"
#include "SquareModelFactory.hpp"
#include "CircleModelFactory.hpp"
#include "Model.hpp"
#include "Cursor.hpp"
#include "Font.hpp"
#include "ProgramScene.hpp"

class Window : public RenderWindow
{
private:
	static const char* fragmentShaderSource;
	static const char* vertexShaderSource;

	Shader* mShader;
	Shader* mScreenShader;
	Cursor* mCursor;
	Font *mFont;
	Font * mSceneFont;
	BaseModelFactory* mBaseModelFactory;
	std::vector<Model> mModels;
	ProgramScene* mScene;

	Model* mDijkstraButton;
	Model* mKruskalButton;
	Model* mResetColorButton;
public:
	Window(const WindowConfiguration& windowConfiguration,
		const OpenGLConfiguration& openGLConfiguration);
	~Window();

protected:
	virtual void onUpdate() override; 
	virtual void onRender() override;
	virtual void onInput(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	virtual void onLoad() override;
	virtual void onCursorMoove(double xPos, double yPos);
	virtual void onMouseClick(int button, int action, int mods);
	virtual void onMouseClickLeft();
	virtual void onMouseClickRight();
	virtual void onScroll(double xOffset, double yOffset);
	virtual void onTyping(GLFWwindow* window, unsigned int codepoint) override;
private:
	bool checkSquareColide(const glm::vec2& pos, const glm::vec2& scale, const glm::vec2& cursorPos);
};

#endif 
