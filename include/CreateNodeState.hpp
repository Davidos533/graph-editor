#ifndef CREATE_NODE_STATE_HPP
#define CREATE_NODE_STATE_HPP

#include "ProgramBaseState.hpp"
#include "CreateArcState.hpp"
#include "InputWeightState.hpp"

class CreateNodeState : public ProgramBaseState
{
public:
	CreateNodeState(ProgramScene* scene);

	virtual void onLeftClick(const glm::vec2 cursorPos) override;
	virtual void onRightClick(const glm::vec2 cursorPos) override;
	virtual void onTyping(const std::string& text) override;
	virtual void onCursorMove(const glm::vec2& cursorPos) override;
	virtual void onUpdate() override;

	~CreateNodeState();
};

#endif 
