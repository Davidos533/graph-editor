#include "InputWeightState.hpp"
#include "CreateNodeState.hpp"

InputWeightState::InputWeightState(ProgramScene* scene, int arcNum)
	:ProgramBaseState(scene), mArcNum(arcNum), mBlinkingMode(BlinkingMode::Darker)
{
}

void InputWeightState::onLeftClick(const glm::vec2 cursorPos)
{
	setDefaultColor();
	resetArcWeightIfNeed();
	mScene->changeState(new CreateNodeState(mScene));
}

void InputWeightState::onRightClick(const glm::vec2 cursorPos)
{
	setDefaultColor();
	resetArcWeightIfNeed();
	mScene->changeState(new CreateNodeState(mScene));
}

void InputWeightState::onTyping(const std::string& text)
{
	if (text == "backspace")
	{
		auto& arcWeight = mScene->mGraphArcs[mArcNum].second.second.first;
		auto newSize = arcWeight.size() - 1;

		if (newSize <= 0)
		{
			arcWeight = "0";
		}
		else
		{
			arcWeight.resize(newSize);
		}
	}
	if (text.size() >= 1 && isdigit(text[0]))
	{
		auto& arcWeight = mScene->mGraphArcs[mArcNum].second.second.first;

		if (arcWeight.size() == 1 && arcWeight[0] == '0')
		{
			arcWeight[0] = text[0];
		}
		else if (arcWeight.size() < 6)
		{
			arcWeight += text[0];	
		}
	}
}

void InputWeightState::onCursorMove(const glm::vec2& cursorPos)
{

}

InputWeightState::~InputWeightState()
{

}

void InputWeightState::setDefaultColor()
{
	mScene->mGraphArcs[mArcNum].second.second.second = 1.0f;
}

void InputWeightState::resetArcWeightIfNeed()
{
	auto& w = mScene->mGraphArcs[mArcNum].second.second.first;

	if (w.size() == 1 && w[0] == '0')
	{
		w = "1";
	}
}

void InputWeightState::onUpdate()
{
	auto &arcColor = mScene->mGraphArcs[mArcNum].second.second.second;
	
	if (arcColor <= mColorBottomBorder)
	{
		mBlinkingMode = BlinkingMode::Lighter;
	}
	else if (arcColor >= mColorTopBorder)
	{
		mBlinkingMode = BlinkingMode::Darker;
	}

	if (mBlinkingMode == BlinkingMode::Darker)
	{
		arcColor -= mBlinkStep;
	}
	else
	{
		arcColor += mBlinkStep;
	}
}
