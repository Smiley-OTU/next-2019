#include "stdafx.h"
#include "Pathing.h"

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

/*inline bool compareNodes(const CPathNode& a, const CPathNode& b) {
	return a.xIndex == b.xIndex && a.yIndex == b.yIndex;
}*/

const std::vector<Cell>& Pathing::aStar(const Cell & start, const Cell & end)
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

		//Remove best node from the open list.
		openList.erase(std::find(openList.begin(), openList.end(), bestNode));


	}
}
