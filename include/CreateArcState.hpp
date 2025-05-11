#ifndef CREATE_ARC_STATE_HPP
#define CREATE_ARC_STATE_HPP

#include "ProgramBaseState.hpp"
#include "CreateNodeState.hpp"

class CreateArcState : public ProgramBaseState
{
public:
	CreateArcState(ProgramScene* scene);

	virtual void onLeftClick(const glm::vec2 cursorPos) override;
	virtual void onRightClick(const glm::vec2 cursorPos) override;
	virtual void onTyping(const std::string& text) override;
	virtual void onCursorMove(const glm::vec2& cursorPos) override;
	virtual void onUpdate() override;

	~CreateArcState();
};

#endif
