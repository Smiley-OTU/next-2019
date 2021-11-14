#include "stdafx.h"
#include "Pathing.h"
#include "Point.h"
#include "SimpleTileMap.h"

namespace Pathing {
	CPoint Pathing::Ricochet(const CSimpleTileMap& map, const CPoint& position, const CPoint& translation)
	{
		CPoint destination{ position + translation };
		EMapValue xTile = map.GetTileMapValue(destination.x, position.y);
		EMapValue yTile = map.GetTileMapValue(position.x, destination.y);
		if (yTile == EMapValue::BORDER || yTile == EMapValue::WALL || yTile == EMapValue::OUTOFBOUNDS)
			destination.y -= translation.y;
		if (xTile == EMapValue::BORDER || xTile == EMapValue::WALL || xTile == EMapValue::OUTOFBOUNDS)
			destination.x -= translation.x;
		return destination;
	}

	inline int cost(Cell a, Cell b)
	{

	}

	std::vector<Cell> Pathing::aStar(const CSimpleTileMap& map, Cell start, Cell end)
	{
		

		//Path finding failed if we've hit this point.
		return std::vector<Cell>{};
	}
}