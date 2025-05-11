#ifndef INPUT_WEIGHT_STATE_HPP
#define INPUT_WEIGHT_STATE_HPP

#include "ProgramBaseState.hpp"

class InputWeightState : public ProgramBaseState
{
	enum class BlinkingMode
	{
		Darker,
		Lighter
	};
	int mArcNum;
	BlinkingMode mBlinkingMode;
	float mBlinkStep = 0.05f;
	float mColorBottomBorder = 0.4f;
	float mColorTopBorder = 1.0f;

public:
	InputWeightState(ProgramScene* scene, int arcNum);

	virtual void onLeftClick(const glm::vec2 cursorPos) override;
	virtual void onRightClick(const glm::vec2 cursorPos) override;
	virtual void onTyping(const std::string& text) override;
	virtual void onCursorMove(const glm::vec2& cursorPos) override;
	virtual void onUpdate() override;

	~InputWeightState();
private:
	void setDefaultColor();
	void resetArcWeightIfNeed();

};

#endif