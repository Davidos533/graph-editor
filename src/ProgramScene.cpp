#include "ProgramScene.hpp"
#include "ProgramBaseState.hpp"
#include "CreateNodeState.hpp"
#include "InputWeightState.hpp"

ProgramScene::ProgramScene(Cursor& curosr, Font& font, float comparePrecision)
	:mCursor(curosr), mComparePrecision(comparePrecision), mCurrentState(ProgramState::CreateNode), mFont(font)
{
	mCircleModleFacotry.scaleVec = glm::vec3(0.05f);
	mCircleModleFacotry.colorVec = glm::vec4(1.0f, 0.09f, 0.03f, 0.5f);

	mArrowModelFactory.colorVec = glm::vec4(0.55f, 0.01f, 0.46f, 0.0f);

	mNodePrototype = mCircleModleFacotry.create();
	mState = new CreateNodeState(this);
}

ProgramScene::~ProgramScene()
{
	delete mNodePrototype;
}

void ProgramScene::onUpdate()
{
	mNodePrototype->setPos(glm::vec3(mCursor.getXNDCCoord(), mCursor.getYNDCCoord(), 0.0f));
	mState->onUpdate();
}

void ProgramScene::onRender(const Shader& shader)
{
	mFont.renderText("Graph Builder\ncreated by David Medvedkov", glm::vec2(-1.0f,0.8f), 0.002f, glm::vec4(1.0f, 0.47f, 0.1f, 1.0f));
	
	for (auto& arcItem : mGraphArcs)
	{
		arcItem.second.first.render(shader);
	}

	int index = 0;
	for (auto& node : mGraphNodes)
	{
		node.render(shader);
		mFont.renderText(std::string(1, static_cast<char>(65 + index)), glm::vec2(node.getPos().x, node.getPos().y), 0.0015f);
		index++;
	}

	index = 0;
	for (auto& arcItem : mGraphArcs)
	{
		mFont.renderText("n:" + std::to_string(index) + ", w:" + arcItem.second.second.first, glm::vec2(arcItem.second.first.getPos().x, arcItem.second.first.getPos().y), 0.0015f, glm::vec4(glm::vec3(arcItem.second.second.second), arcItem.second.first.getColor().w));
		index++;
	}

	mNodePrototype->render(shader);
}

void ProgramScene::onClickLeft(float xPos, float yPos)
{
	mState->onLeftClick(glm::vec2(xPos, yPos));
}

void ProgramScene::onClickRight(float xPos, float yPos)
{
	mState->onRightClick(glm::vec2(xPos, yPos));
}

void ProgramScene::onCursorMoove(const glm::vec2& screenCoords, const glm::vec2& worldCords)
{
	for (auto& arcItem : mGraphArcs)
	{
		if (arcItem.first.second == -1)
		{
			auto arcStartNode = mGraphNodes[arcItem.first.first];
			auto arcStartNodePos = arcStartNode.getPos();
			auto wc3 = glm::vec3(worldCords, 0.0f);
			auto arcLength = glm::distance(wc3, arcStartNodePos);
			auto middlePoint = glm::vec3((wc3.x + arcStartNodePos.x) / 2, (wc3.y + arcStartNodePos.y) / 2, 0.0f);

			auto startVec = glm::vec2(0.0f, 1.0f);
			auto dirVec = glm::normalize(glm::vec2(wc3.x - arcStartNodePos.x, wc3.y - arcStartNodePos.y));

			auto angle = glm::orientedAngle(startVec, dirVec);

			arcItem.second.first.setScale(glm::vec3(0.2f, arcLength, 1.0f));
			arcItem.second.first.setPos(middlePoint);
			arcItem.second.first.setRotate(angle , glm::vec3(0.0f, 0.0f, 1.0f));
			arcItem.second.first.setColor(glm::vec4(0.55f, 0.01f, 0.46f, 0.5f));

			break;
		}
	}

	for (auto& node : mGraphNodes)
	{
		auto pos = glm::vec2(node.getPos());
		auto& radius = node.getSclae().x;
		auto d = glm::distance(pos, worldCords);
		if (d < radius * 2)
		{
			node.isSelect(true);
			continue;
		}
		node.isSelect(false);
	}

	for (auto& arcItem : mGraphArcs)
	{
		if (arcItem.first.second != -1)
		{
			auto widht = arcItem.second.first.getSclae().x / 2;

			auto radius = mGraphNodes[arcItem.first.first].getSclae().x;
			auto startNodePos = glm::vec2(mGraphNodes[arcItem.first.first].getPos());
			auto endNodePos = glm::vec2(mGraphNodes[arcItem.first.second].getPos());

			auto d = glm::distance(glm::closestPointOnLine(worldCords, startNodePos, endNodePos), worldCords);

			auto d1 = glm::distance(startNodePos, worldCords);
			auto d2 = glm::distance(endNodePos, worldCords);

			if (d1 <= radius || d2 <= radius)
			{
				continue;
			}

			if (d <= widht)
			{
				arcItem.second.first.isSelect(true);
				continue;
			}
		}
		arcItem.second.first.isSelect(false);
	}
}

void ProgramScene::onTyping(const std::string& text)
{
	mState->onTyping(text);
}

void ProgramScene::onInput(int key)
{

}

void ProgramScene::changeState(ProgramBaseState* state)
{
	if (mState != nullptr)
	{
		delete mState;
	}

	mState = state;
}

bool ProgramScene::startDijkstra()
{
	resetArcsColors();
	auto arcInfo = prepareArcInfo();

	auto selectedArcs = mIncedenceMatrixGraphService.dijkstra(mGraphNodes.size(), mGraphArcs.size(), arcInfo);

	processSelectedArcs(selectedArcs);

	delete[] arcInfo;
	delete[] selectedArcs.second;

	return false;
}

bool ProgramScene::isCompletedGraph()
{
	return !mGraphNodes.empty();
}

bool ProgramScene::startKruskal()
{
	resetArcsColors();
	auto arcInfo = prepareArcInfo();

	auto selectedArcs = mIncedenceMatrixGraphService.kruskal(mGraphNodes.size(), mGraphArcs.size(), arcInfo);

	processSelectedArcs(selectedArcs);

	delete[] arcInfo;
	delete[] selectedArcs.second;

	return false;
}

void ProgramScene::resetArcsColors()
{
	for (auto& arcItem : mGraphArcs)
	{
		arcItem.second.first.setColor(glm::vec4(0.55f, 0.01f, 0.46f, 1.0f));
	}
}

ArcInfo* ProgramScene::prepareArcInfo()
{
	ArcInfo* arcInfo = new ArcInfo[mGraphArcs.size()];

	for (int i = 0; i < mGraphArcs.size(); i++)
	{
		auto& nodesInfo = mGraphArcs[i].first;
		auto arcWeight = std::stoi(mGraphArcs[i].second.second.first);
		arcInfo[i] = ArcInfo(ArcNodes(nodesInfo.first, nodesInfo.second), arcWeight);
	}

	return arcInfo;
}

void ProgramScene::processSelectedArcs(ArcResult selectedArcs)
{
	for (auto& arcItem : mGraphArcs)
	{
		for (int i = 0; i < selectedArcs.first; i++)
		{
			auto& arcNodes = selectedArcs.second[i];
			if ((arcItem.first.first == arcNodes.first && arcItem.first.second == arcNodes.second)
				|| (arcItem.first.first == arcNodes.second && arcItem.first.second == arcNodes.first))
			{
				arcItem.second.first.setColor(glm::vec4(0.0f,0.0f,1.0f,1.0f));
				break;
			}
		}
	}

	/*std::vector<std::pair<int, int>> removeIds;

	int index = 0;
	for (auto& arcItem : mGraphArcs)
	{
		bool deleteFlag = true;
		for (int i = 0; i < selectedArcs.first; i++)
		{
			auto& arcNodes = selectedArcs.second[i];
			if ((arcItem.first.first == arcNodes.first && arcItem.first.second == arcNodes.second)
				|| (arcItem.first.first == arcNodes.second && arcItem.first.second == arcNodes.first))
			{
				deleteFlag = false;
				break;
			}
		}

		if (deleteFlag)
		{
			removeIds.push_back(std::pair<int, int>(arcItem.first.first, arcItem.first.second));
		}

		index++;
	}

	for (auto& remId : removeIds)
	{
		mGraphArcs.erase(
			std::remove_if
			(mGraphArcs.begin(),
				mGraphArcs.end(),
				[remId](ArcItem& el)
				{
					return (el.first.first == remId.first && el.first.second == remId.second) || (el.first.first == remId.second && el.first.second == remId.first);
				}), mGraphArcs.end());
	}*/
}

const ColideInfo& ProgramScene::colideWith(const glm::vec2 cursorPos)
{
	int index = 0;
	for (auto& node : mGraphNodes)
	{
		auto pos = glm::vec2(node.getPos());
		auto& radius = node.getSclae().x;
		auto d = glm::distance(pos, cursorPos);
		if (d < radius * 2)
		{
			return ColideInfo(ColideType::Node, index);
		}
		index++;
	}

	index = 0;
	for (auto& arcItem : mGraphArcs)
	{
		if (arcItem.first.second != -1)
		{
			auto widht = arcItem.second.first.getSclae().x / 2;

			auto radius = mGraphNodes[arcItem.first.first].getSclae().x;
			auto startNodePos = glm::vec2(mGraphNodes[arcItem.first.first].getPos());
			auto endNodePos = glm::vec2(mGraphNodes[arcItem.first.second].getPos());

			auto d = glm::distance(glm::closestPointOnLine(cursorPos, startNodePos, endNodePos), cursorPos);

			auto d1 = glm::distance(startNodePos, cursorPos);
			auto d2 = glm::distance(endNodePos, cursorPos);

			if (d1 <= radius || d2 <= radius)
			{
				continue;
			}

			if (d <= widht)
			{
				return ColideInfo(ColideType::Arc, 0, arcItem.first.first, arcItem.first.second, index);
			}
			
		}
		index++;
	}

	return ColideInfo(ColideType::Nothing);
}

int ProgramScene::createNode(const glm::vec2 pos)
{
	if (mGraphNodes.size() < mMaxCountOfNodes)
	{
		mCircleModleFacotry.posVec = glm::vec3(pos, 0.0f);
		mCircleModleFacotry.colorVec = glm::vec4(1.0f, 0.09f, 0.03f, 1.0f);
		mGraphNodes.push_back(*mCircleModleFacotry.create());

		return mGraphNodes.size()-1;
	}

	return -1;
}

void ProgramScene::deleteNode(int nodeId)
{
	mGraphArcs.erase(std::remove_if(mGraphArcs.begin(), mGraphArcs.end(), [nodeId](ArcItem &el) { return el.first.first == nodeId || el.first.second == nodeId; }), mGraphArcs.end());
	mGraphNodes.erase(mGraphNodes.begin() + nodeId);

	for (auto& arcItem : mGraphArcs)
	{
		if (arcItem.first.first > nodeId)
		{
			arcItem.first.first--;
		}
		if (arcItem.first.second > nodeId)
		{
			arcItem.first.second--;
		}
	}
}

void ProgramScene::deleteArc(int arcId)
{
	mGraphArcs.erase(mGraphArcs.begin() + arcId);
}

void ProgramScene::startArc(int nodeNum)
{
	mGraphArcs.push_back(ArcItem(ArcNodesNums(nodeNum, -1), ArcWeightColor(* mArrowModelFactory.create(),WeightColor("1",1.0f))));
}

bool ProgramScene::finishArc(int nodeNum)
{
	for (auto& arcItem : mGraphArcs)
	{
		if (arcItem.first.second == -1 && arcItem.first.first != nodeNum)
		{
			arcItem.first.second = nodeNum;

			arcItem.second.first.setColor(glm::vec4(0.55f, 0.01f, 0.46f, 1.0f));
			return true;
		}
	}
}

void ProgramScene::removeArc()
{
	int index = 0;
	for (auto& arcItem : mGraphArcs)
	{
		if (arcItem.first.second == -1)
		{
			break;
		}
		index++;
	}

	mGraphArcs.erase(mGraphArcs.begin() + index);
}
