#ifndef PROGRAM_BASE_STATE
#define PROGRAM_BASE_STATE

#include "ProgramScene.hpp"

class ProgramBaseState
{
protected:
	ProgramScene* mScene;
public:
	ProgramBaseState(ProgramScene* scene);

	virtual void onLeftClick(const glm::vec2 cursorPos) = 0;
	virtual void onRightClick(const glm::vec2 cursorPos) = 0;
	virtual void onTyping(const std::string& text) = 0;
	virtual void onCursorMove(const glm::vec2& cursorPos) = 0;
	virtual void onUpdate() = 0;
	
	~ProgramBaseState();
};

#endif