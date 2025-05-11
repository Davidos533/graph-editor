#ifndef PROGRAM_SCENE_HPP
#define PROGRAM_SCENE_HPP

#include <vector>
#include <algorithm>

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/closest_point.hpp>

#include "Model.hpp"
#include "CircleModelFactory.hpp"
#include "SquareModelFactory.hpp"
#include "Cursor.hpp"
#include "ProgramState.hpp"
#include "ColideInfo.hpp"
#include "ArrowModelFactory.hpp"

#include "Font.hpp"

#include "IncedenceMatrixGraphService.hpp"

class ProgramBaseState;
class CreateNodeState;
class CreateArcState;
class InputWeightState;

using ArcNodesNums = std::pair<int, int>;
using WeightColor = std::pair<std::string, float>;
using ArcWeightColor = std::pair<Model, WeightColor>;
using ArcItem = std::pair<ArcNodesNums, ArcWeightColor>;

class ProgramScene
{
private:
	std::vector<Model> mGraphNodes;
	std::vector<ArcItem> mGraphArcs;

	CircleModleFacotry mCircleModleFacotry;
	SquareModelFactory mSquareModelFactory;
	ArrowModelFactory mArrowModelFactory;

	Cursor &mCursor;
	Font &mFont;

	Model* mNodePrototype = nullptr;
	Model* mCurrentNewArc = nullptr;
	
	ProgramState mCurrentState;
	float mComparePrecision;
	ProgramBaseState* mState;
	int mMaxCountOfNodes = 26;

	IncedenceMatrixGraphService mIncedenceMatrixGraphService;
public:
	ProgramScene(Cursor &cursor, Font& font, float comparePrecision = 0.01f);

	void onUpdate();
	void onRender(const Shader& shader);

	void onClickLeft(float xPos, float yPos);
	
	void onClickRight(float xPos, float yPos);
	
	void onCursorMoove(const glm::vec2& screenCoords, const glm::vec2& worldCords);

	void onTyping(const std::string& text);

	void onInput(int key);

	void changeState(ProgramBaseState* state);

	bool isCompletedGraph();
	bool startDijkstra();
	bool startKruskal();
	void resetArcsColors();
	ArcInfo* prepareArcInfo();
	void processSelectedArcs(ArcResult selectedArcs);

	~ProgramScene();

	friend ProgramBaseState;
	friend CreateNodeState;
	friend CreateArcState;
	friend InputWeightState;
private:
	const ColideInfo& colideWith(const glm::vec2 cursorPos);
	int createNode(const glm::vec2 pos);
	void deleteNode(int nodeNum);
	void deleteArc(int arcId);
	void startArc(int nodeNum);
	bool finishArc(int nodeNum);
	void removeArc();
};

#endif