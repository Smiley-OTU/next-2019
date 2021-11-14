#pragma once
#include "Cell.h"
#include <vector>
struct CPoint;
class CSimpleTileMap;
namespace Pathing {
	//Returns a vector of tile grid indices to traverse to get from start to end.
	std::vector<Cell> aStar(const CSimpleTileMap& map, Cell start, Cell end);

	CPoint Ricochet(const CSimpleTileMap& map, const CPoint& position, const CPoint& translation);
}