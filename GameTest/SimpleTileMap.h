#ifndef _SIMPLE_TILE_MAP_H
#define _SIMPLE_TILE_MAP_H

#define MAP_SIZE 16
#include "Cell.h"
#include "Point.h"
#include <array>

enum EMapValue : int
{
	OUTOFBOUNDS = -1,
	BORDER,
	AIR,
	WALL,
	POWER_UP,
	NUM_TILE_TYPES
};

struct CTile {
	//Height is for the 3D projection, scale is for 2D.
	const float r, g, b, height, scale;
	static const CTile tiles[NUM_TILE_TYPES];
};

class CSimpleTileMap
{
public:
    // Creates a tile map of mapSize by mapSize.
	CSimpleTileMap(const int mapSize);

    // Clear each tile of the map.
    void Clear(EMapValue clearValue = EMapValue::WALL );

    // Draws the map from a bird's eye view.
    void Render() const;

    EMapValue GetTileMapValue(const int x, const int y)  const;
    EMapValue GetTileMapValue(const float fx, const float fy) const;
    bool SetTileMapValue(const int x, const int y, EMapValue v);

	Cell GetCell(const CPoint& point) const; 
    int GetCellIndex(const Cell& cell) const;
    CPoint GetCellCentre(const Cell& cell) const;
	void DrawTile(const Cell& cell, float r, float g, float b) const;

    int GetMapSize() const;
	float GetTileWidth() const;
	float GetTileHeight() const;

private:
    void RandomMap(const float targetFloorPercentage, const int maxTunnelLength);
    int GetNewDirection(const int currentRow, const int currentColumn, int currentDir) const;

    const int m_mapSize;
    const float m_tileWidth;
    const float m_tileHeight;
    std::array<std::array<EMapValue, MAP_SIZE>, MAP_SIZE> m_tileValues;
};
#endif
