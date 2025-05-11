#include "CreateNodeState.hpp"

CreateNodeState::CreateNodeState(ProgramScene* scene)
	:ProgramBaseState(scene)
{
}

CreateNodeState::~CreateNodeState()
{

}

void CreateNodeState::onUpdate()
{
}

void CreateNodeState::onLeftClick(const glm::vec2 cursorPos)
{
	auto colideWith = mScene->colideWith(cursorPos);

	switch (colideWith.mColideType)
	{
		case ColideType::Nothing:
		{
			mScene->createNode(cursorPos);
		}
		break;
		case ColideType::Node:
		{
			mScene->startArc(colideWith.NodeId);
			mScene->changeState(new CreateArcState(mScene));
		}
		break;
		case ColideType::Arc:
		{
			mScene->changeState(new InputWeightState(mScene, colideWith.ArcId));
		}
		break;
	}
}

void CreateNodeState::onRightClick(const glm::vec2 cursorPos)
{
	auto colideWith = mScene->colideWith(cursorPos);

	switch (colideWith.mColideType)
	{
		case ColideType::Node:
		{
			mScene->deleteNode(colideWith.NodeId);
		}
		break;
		case ColideType::Arc:
		{
			mScene->deleteArc(colideWith.ArcId);
		}
		break;
	}
}

void CreateNodeState::onTyping(const std::string& text)
{
}

void CreateNodeState::onCursorMove(const glm::vec2& cursorPos)
{
}
