#include "CreateArcState.hpp"

CreateArcState::CreateArcState(ProgramScene* scene)
	:ProgramBaseState(scene)
{

}


CreateArcState::~CreateArcState()
{

}

void CreateArcState::onUpdate()
{
}

void CreateArcState::onLeftClick(const glm::vec2 cursorPos)
{
	auto colideWith = mScene->colideWith(cursorPos);

	switch (colideWith.mColideType)
	{
		case ColideType::Nothing:
		{
			auto nodeId = mScene->createNode(cursorPos);
			if (nodeId != -1)
			{
				mScene->finishArc(nodeId);
				mScene->changeState(new CreateNodeState(mScene));
			}
		}
		break;
		case ColideType::Node:
		{
			auto res = mScene->finishArc(colideWith.NodeId);
			
			if (res)
			{
				mScene->changeState(new CreateNodeState(mScene));
			}
		}
		break;
		case ColideType::Arc:
		{

		}
		break;
	}
}

void CreateArcState::onRightClick(const glm::vec2 cursorPos)
{
	auto colideWith = mScene->colideWith(cursorPos);

	switch (colideWith.mColideType)
	{
		case ColideType::Nothing:
		{
			mScene->removeArc();
			mScene->changeState(new CreateNodeState(mScene));
		}
		break;
		case ColideType::Node:
		{

		}
		break;
	}
}

void CreateArcState::onTyping(const std::string& text)
{
}

void CreateArcState::onCursorMove(const glm::vec2& cursorPos)
{
}
