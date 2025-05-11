#ifndef INCIDENCE_MATRIX_GRAPH_SERVICE_HPP
#define INCIDENCE_MATRIX_GRAPH_SERVICE_HPP

#include <iostream>
#include <string>
#include <utility>
#include <vector>

using ArcNodes = std::pair<int, int>;
using ArcInfo = std::pair<ArcNodes, int>;
using ArcResult = std::pair<int, ArcNodes*>;

using DijkstraResultTable = std::vector<std::vector<int>>;

class IncedenceMatrixGraphService
{
public:
	IncedenceMatrixGraphService();

	ArcResult kruskal(int countOfNodes, int countOfArcs, ArcInfo* arcsNodes);
	ArcResult dijkstra(int countOfNodes, int countOfArcs, ArcInfo* arcsNodes);

	~IncedenceMatrixGraphService();
private:
	void initMatrix(int **&matrix, int countOfNodes);
	void fillMatrixKruskal(int **matrix, int countOfNodes, int countOfArcs, ArcInfo* arcsNodes);
	void fillMatrixDijkstra(int **matrix, int countOfNodes, int countOfArcs, ArcInfo* arcsNodes);

	/// <summary>
	/// add new row to Dijkstras result table incidence matrix data-driven
	/// </summary>
	/// <param name="resultTable">Dijkstra result table</param>
	/// <param name="matrix">incidence matrix</param>
	/// <param name="countOfNodes">count of graph nodes</param>
	/// <returns>is table finished</returns>
	bool addRowToDijkstraResultTable(DijkstraResultTable& resultTable, int **matrix,int countOfNodes);

	// Kruskal
	ArcInfo getNextArcNodesAndMinWeight(int currentWeight, int **matrix, int countOfNodes, int **resultMatrix);
	bool isClosedWithNode(int checkingNodeNum, int **resultMatrix, int countOfNodes, int nextNodeNum, int previousNodeNum);
	
	ArcResult convertDijkstraResultTableToResultArcs(DijkstraResultTable& table, int countOfNodes);

	std::string getMatrixView(int** matrix, int size);
	std::string getDijkstraTableView(DijkstraResultTable table);
};

#endif