#include "stdafx.h"
#include "Pathing.h"
#include "SimpleTileMap.h"

struct CPathNode {
	CPathNode(const Cell& cell) : xIndex(cell.first), yIndex(cell.second) {}
	float f() { return g + h; }
	float g, h;
	int xIndex;
	int yIndex;
	CPathNode* parent = nullptr;

	bool operator==(const CPathNode& pathNode) {
		return xIndex == pathNode.xIndex && yIndex == pathNode.yIndex;
	}
};

std::vector<CPathNode> getNeighbours(const CSimpleTileMap& map, int xIndex, int yIndex) {
	std::vector<CPathNode> neighbours;
	const static char numAdjacent = 4;
	CPathNode pendingNeighbours[numAdjacent] = {
		CPathNode{std::make_pair(xIndex - 1, yIndex)},	//Left
		CPathNode{std::make_pair(xIndex + 1, yIndex)},	//Right
		CPathNode{std::make_pair(xIndex, yIndex - 1)},	//Up
		CPathNode{std::make_pair(xIndex, yIndex + 1)},	//Down
	};

	for (char i = 0; i < numAdjacent; i++) {
		const CPathNode& pn = pendingNeighbours[i];
		EMapValue tileValue = map.GetTileMapValue(pn.xIndex, pn.yIndex);
		if (tileValue != EMapValue::BORDER && tileValue != EMapValue::WALL && tileValue != EMapValue::OUTOFBOUNDS)
			neighbours.push_back(pn);
	}

	return neighbours;
}

std::vector<Cell> Pathing::aStar(const CSimpleTileMap& map, const Cell & start, const Cell & end)
{
	std::vector<CPathNode> closedList;
	std::vector<CPathNode> openList{ CPathNode(start) };

	while (openList.size() > 0) {
		//Best scoring node index.
		CPathNode& bestNode = openList[0];

		//See if the current node is better than the current best node.
		for (auto& node : openList) {
			if (node.f() < bestNode.f())
				bestNode = node;
		}

		if (bestNode.xIndex == end.first && bestNode.yIndex == end.second) {
			std::vector<Cell> path;
			while (bestNode.parent) {
				path.push_back(std::make_pair(bestNode.xIndex, bestNode.yIndex));
				bestNode = *bestNode.parent;
			}

			std::reverse(path.begin(), path.end());
			return path;
		}

		//Add the best node to the closed list and remove it from the open list.
		closedList.push_back(bestNode);
		openList.erase(std::find(openList.begin(), openList.end(), bestNode));


	}
}
