#include "stdafx.h"
#include "Pathing.h"
#include "SimpleTileMap.h"

struct CPathNode {
	CPathNode(const Cell& cell) : xIndex(cell.first), yIndex(cell.second) {}
	float f() { return g + h; }
	int g = 1;
	float h = 0.0f;
	int xIndex = 0;
	int yIndex = 0;
	CPathNode* parent = nullptr;

	bool operator==(const CPathNode& pathNode) {
		return xIndex == pathNode.xIndex && yIndex == pathNode.yIndex;
	}
};

inline std::vector<CPathNode> getNeighbours(const CSimpleTileMap& map, int xIndex, int yIndex) {
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

//Manhattan distance as my heuristic.
inline float hValue(const Cell& cell1, const Cell& cell2) {
	return abs(cell2.first - cell1.first) + abs(cell2.second - cell1.second);
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

		//Traverse the list of path nodes from end to start then reverse it!
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

		//Fetch surrounding nodes.
		std::vector<CPathNode> neighbours = getNeighbours(map, bestNode.xIndex, bestNode.yIndex);

		for (CPathNode& node : neighbours) {
			//Make sure current node hasn't already been processed (ensure not in closed list).
			if (std::find(closedList.begin(), closedList.end(), node) != closedList.end())
				continue;

			//Find the best scoring neighbour.
			int gScore = bestNode.g + 1;
			bool gScoreIsBest = false;

			//Figure out if we've already processed it.
			bool onList = false;
			for (CPathNode& openNode : openList) {
				if (openNode == node)
					onList = true;
			}

			//If we're dealing with a new node, its guaranteed to be the best.
			if (!onList) {
				gScoreIsBest = true;
				node.h = hValue(std::make_pair(node.xIndex, node.yIndex), end);
				openList.push_back(node);
			}

			//Otherwise, re-evaluate its g score.
			else if (gScore < node.g)
				gScoreIsBest = true;

			//Store the list leading up to the best node so far.
			if (gScoreIsBest) {
				node.parent = &bestNode;
				node.g = gScore;
			}
		}
	}

	//Path finding failed if we've hit this point.
	return std::vector<Cell>{};
}
