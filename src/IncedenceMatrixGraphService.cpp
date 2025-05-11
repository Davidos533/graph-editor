#include "IncedenceMatrixGraphService.hpp"

IncedenceMatrixGraphService::IncedenceMatrixGraphService()
{
}

ArcResult IncedenceMatrixGraphService::kruskal(int countOfNodes, int countOfArcs, ArcInfo* arcsNodes)
{
	int** matrix = nullptr;
	int** resultMatrix = nullptr;

	initMatrix(matrix, countOfNodes);
	initMatrix(resultMatrix, countOfNodes);
	fillMatrixKruskal(matrix, countOfNodes, countOfArcs, arcsNodes);

	int minWeight = 0;

	int countOfSelectedArcs = 0;

	std::cerr << "\n" << getMatrixView(matrix, countOfNodes) << "\n";

	while (true)
	{
		ArcInfo arcInfo = getNextArcNodesAndMinWeight(minWeight, matrix, countOfNodes, resultMatrix);
		minWeight = arcInfo.second;

		if (minWeight == -1)
		{
			break;
		}

		resultMatrix[arcInfo.first.first][arcInfo.first.second] = matrix[arcInfo.first.first][arcInfo.first.second];
		resultMatrix[arcInfo.first.second][arcInfo.first.first] = matrix[arcInfo.first.first][arcInfo.first.second];

		// check is finded min arc can connect without creating closing graph
		if (!isClosedWithNode(arcInfo.first.first, resultMatrix, countOfNodes, arcInfo.first.first, arcInfo.first.first))
		{
			countOfSelectedArcs++;
		}
		else
		{
			resultMatrix[arcInfo.first.first][arcInfo.first.second] = -1;
			resultMatrix[arcInfo.first.second][arcInfo.first.first] = -1;
		}

		std::cerr << "\n" << getMatrixView(resultMatrix, countOfNodes) << "\n";

	}

	ArcNodes* resultArcsNodes = new ArcNodes[countOfSelectedArcs];

	for (int row = 0, index = 0; row < countOfNodes; row++)
	{
		for (int col = row + 1; col < countOfNodes; col++)
		{
			if (resultMatrix[row][col] != 0 && resultMatrix[row][col] != -1)
			{
				resultArcsNodes[index] = ArcNodes(row, col);
				index++;
			}
		}
	}

	for (int i = 0; i < countOfNodes; i++)
	{
		delete[] matrix[i];
	}

	delete[] matrix;

	for (int i = 0; i < countOfNodes; i++)
	{
		delete[] resultMatrix[i];
	}

	delete[] resultMatrix;

	return ArcResult(countOfSelectedArcs, resultArcsNodes);
}

ArcResult IncedenceMatrixGraphService::dijkstra(int countOfNodes, int countOfArcs, ArcInfo* arcsNodes)
{
	int** matrix = nullptr;
	DijkstraResultTable  dijkstraResultTable;

	initMatrix(matrix, countOfNodes);
	fillMatrixDijkstra(matrix, countOfNodes, countOfArcs, arcsNodes);

	std::cerr << "\n" << getMatrixView(matrix, countOfNodes) << "\n";

	while (!addRowToDijkstraResultTable(dijkstraResultTable, matrix, countOfNodes));

	auto result = convertDijkstraResultTableToResultArcs(dijkstraResultTable, countOfNodes);

	std::cerr << "\n" << getDijkstraTableView(dijkstraResultTable) << "\n";

	for (int i = 0; i < countOfNodes; i++)
	{
		delete[] matrix[i];
	}

	delete[] matrix;

	return result;
}

IncedenceMatrixGraphService::~IncedenceMatrixGraphService()
{
}

void IncedenceMatrixGraphService::initMatrix(int**& matrix, int countOfNodes)
{
	matrix = new int* [countOfNodes * countOfNodes];

	for (int row = 0; row < countOfNodes; row++)
	{
		matrix[row] = new int[countOfNodes];

		for (int col = 0; col < countOfNodes; col++)
		{
			matrix[row][col] = 0;
		}
	}
}

void IncedenceMatrixGraphService::fillMatrixKruskal(int** matrix, int countOfNodes, int countOfArcs, ArcInfo* arcsNodes)
{
	for (int arcId = 0; arcId < countOfArcs; arcId++)
	{
		auto nodeIds = arcsNodes[arcId].first;
		auto weight = arcsNodes[arcId].second;

		matrix[nodeIds.second][nodeIds.first] = weight;
		matrix[nodeIds.first][nodeIds.second] = weight;
	}
}

void IncedenceMatrixGraphService::fillMatrixDijkstra(int** matrix, int countOfNodes, int countOfArcs, ArcInfo* arcsNodes)
{
	for (int arcId = 0; arcId < countOfArcs; arcId++)
	{
		auto nodeIds = arcsNodes[arcId].first;
		auto weight = arcsNodes[arcId].second;

		matrix[nodeIds.first][nodeIds.second] = weight;
	}
}

bool IncedenceMatrixGraphService::addRowToDijkstraResultTable(DijkstraResultTable& resultTable, int** matrix, int countOfNodes)
{
	bool isNewTable = resultTable.size() == 0;

	if (!isNewTable)
	{
		// check is table finished 
		// get last row selected node id, check is that last node
		auto& lastRow = resultTable[resultTable.size() - 1];
		if (lastRow[countOfNodes] == countOfNodes - 1)
		{
			return true;
		}
	}

	// add new row (+1 for current row selected weight)
	resultTable.push_back(std::vector<int>(countOfNodes + 1, -1));

	// this first row
	if (isNewTable)
	{
		resultTable[0][0] = 0;
		resultTable[0][countOfNodes] = 0;

		if (countOfNodes == 1)
		{

			return true;
		}

		return false;
	}

	// fill based on previous row
	auto& previusRow = resultTable[resultTable.size() - 2];
	auto& lastRow = resultTable[resultTable.size() - 1];
	auto selectedNodeId = previusRow[countOfNodes];
	auto selectedNodeWeight = previusRow[selectedNodeId];

	int index = 0;
	for (auto previousRowElement : previusRow)
	{
		// last element
		if (index == countOfNodes)
		{
			break;
		}

		// passed nodes
		if (previousRowElement == -2 || index == selectedNodeId)
		{
			lastRow[index] = -2;
			index++;
			continue;
		}

		auto currentArcWeight = matrix[selectedNodeId][index];
		auto newWeight = currentArcWeight + selectedNodeWeight;

		// inaccessible node
		if (previousRowElement == -1 && currentArcWeight == 0)
		{
			lastRow[index] = -1;
		}
		// accessed current node
		else if (previousRowElement == -1 && currentArcWeight != 0)
		{
			lastRow[index] = newWeight;
		}
		// inaccessible by current path node, but was accessed earlier
		else if (previousRowElement != -1 && currentArcWeight == 0)
		{
			lastRow[index] = previousRowElement;
		}
		// accessed current node and previous
		else if (previousRowElement != -1 && currentArcWeight != 0)
		{
			
			// previous path shorter than current
			if (newWeight > previousRowElement)
			{
				lastRow[index] = previousRowElement;
			}
			// current path shorter or equals thane previous
			else if (newWeight <= previousRowElement)
			{
				lastRow[index] = newWeight;
			}
		}

		index++;
	}

	int min = -3;
	int minIndex = 0;
	int mindex = 0;
	// determine current
	for (auto lastRowElement : lastRow)
	{
		if ((lastRowElement != -1 && lastRowElement != -2) && (min == -3 || ((min > lastRowElement))))
		{
			min = lastRowElement;
			minIndex = mindex;
		}
		mindex++;
	}

	lastRow[countOfNodes] = minIndex;

	return false;
}

ArcInfo IncedenceMatrixGraphService::getNextArcNodesAndMinWeight(int currentWeight, int** matrix, int countOfNodes, int** resultMatrix)
{
	int min = -1;
	ArcNodes an;

	for (int row = 0; row < countOfNodes; row++)
	{
		for (int col = 0; col < countOfNodes; col++)
		{
			auto weight = matrix[row][col];

			if (currentWeight != 0 && weight == currentWeight && resultMatrix[row][col] == 0)
			{
				return ArcInfo(ArcNodes(row, col), weight);
			}

			if (((min > weight || min == -1) && weight > currentWeight))
			{
				min = weight;
				an = ArcNodes(row, col);
			}
		}
	}

	return ArcInfo(an, min);
}

bool IncedenceMatrixGraphService::isClosedWithNode(int checkingNodeNum, int** resultMatrix, int countOfNodes, int nextNodeNum, int previousNodeNum)
{
	for (int startNodeNum = nextNodeNum; startNodeNum < countOfNodes; startNodeNum++)
	{
		for (int endNodeNum = 0; endNodeNum < countOfNodes; endNodeNum++)
		{
			if (endNodeNum != previousNodeNum && resultMatrix[startNodeNum][endNodeNum] != 0 && resultMatrix[startNodeNum][endNodeNum] != -1)
			{
				if (endNodeNum == checkingNodeNum)
				{
					return true;
				}

				if (isClosedWithNode(checkingNodeNum, resultMatrix, countOfNodes, endNodeNum, startNodeNum))
				{
					return true;
				}
			}
		}
		return false;
	}
}

ArcResult IncedenceMatrixGraphService::convertDijkstraResultTableToResultArcs(DijkstraResultTable& table, int countOfNodes)
{
	std::vector<int> pathNodes;

	auto tableSize = table.size();

	int currentNodeId = table[tableSize - 1][countOfNodes];
	int currentNodeWeight = table[tableSize - 1][currentNodeId];

	pathNodes.push_back(currentNodeId);

	for (int rowNumber = tableSize - 1; rowNumber >= 0; rowNumber--)
	{
		if (table[rowNumber][countOfNodes] != currentNodeId && table[rowNumber][currentNodeId] != currentNodeWeight)
		{
			currentNodeId = table[rowNumber][countOfNodes];
			currentNodeWeight = table[rowNumber][currentNodeId];

			pathNodes.push_back(currentNodeId);
		}
	}

	int arcNodesSize = pathNodes.size() - 1;

	ArcNodes* arcNodes = new ArcNodes[arcNodesSize];

	for (int i = 0; i < arcNodesSize; i++)
	{
		arcNodes[i] = ArcNodes(pathNodes[i], pathNodes[i + 1]);
	}

	return ArcResult(arcNodesSize, arcNodes);
}

std::string IncedenceMatrixGraphService::getMatrixView(int** matrix, int size)
{
	std::string result;

	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
		{
			result += std::to_string(matrix[row][col]) + "\t";
		}
		result += "\n";
	}

	return result;
}

std::string IncedenceMatrixGraphService::getDijkstraTableView(DijkstraResultTable table)
{
	std::string result;

	for (auto& row : table)
	{
		for (auto colItem : row)
		{
			result += std::to_string(colItem) + "\t";
		}

		result += "\n";
	}

	return result;
}
