#pragma once
#include "SimpleTileMap.h"

namespace Pathing {
    using Path = std::vector<Cell>;

    // Returns the resultant position based on speeed along a path from start to end
    CPoint FollowPath(const CPoint& start, const CPoint& end, float speed, const CSimpleTileMap& map);

    // Returns a vector of tile grid indices to traverse to get from start to end (using A*).
    Path FindPath(const Cell& start, const Cell& end, const CSimpleTileMap& map);

    // Render path tiles
    void DrawPath(const Path& path, const CSimpleTileMap& map);

    // Returns the collision response given a point and its desired translation.
    CPoint Ricochet(const CPoint& position, const CPoint& translation, const CSimpleTileMap& map);

    // Returns adjacent cells which can be moved to (tiles who's value is EMapValue::AIR).
    std::vector<Cell> GetNeighbours(const Cell& cell, const CSimpleTileMap& map);

    // Flatten 2d position into 1d index on an NxN grid
    int GetCellIndex(const Cell& cell, const CSimpleTileMap& map);
};