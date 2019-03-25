#pragma once
#include "SimpleTileMap.h"

namespace Pathing {
	//Returns a vector of tile grid indices to traverse to get from start to end.
	std::vector<Cell> aStar(const CSimpleTileMap& map, const Cell& start, const Cell& end);
	//Have a separate algorithm to traverse the from current index to desired index by moving along a line.
	//If index == pacIndex, position += toPacman, else postition += toNextTileCentre.
}