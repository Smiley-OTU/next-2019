#ifndef _SIMPLE_TILE_MAP_H
#define _SIMPLE_TILE_MAP_H

#define MAP_SIZE 16
#include "Cell.h"
#include "Point.h"
#include <array>

enum EMapValue : int
{
	OUTOFBOUNDS = -1,   // If a GetTileMapValue() call is ourside the bounds of the map it will return this.
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

    // Clear each tile of the map
    void Clear(EMapValue clearValue = EMapValue::WALL );

    // Bird's eye view of the map
    void Render() const;

    EMapValue GetTileMapValue(const int x, const int y)  const;
    EMapValue GetTileMapValue(const float fx, const float fy) const;
    bool SetTileMapValue(const int x, const int y, EMapValue v);

	Cell GetCell(float x, float y) const;
	Cell GetCell(const CPoint& point) const; 
    int GetCellIndex(const Cell& cell) const;
    CPoint GetCellCentre(const Cell& cell) const;
	void DrawTile(const Cell& cell, float r, float g, float b) const;

    int GetMapSize() const;
	float GetTileWidth() const;
	float GetTileHeight() const;

private:
    //--------------------------------------------------------------------------------------------
    // This will generate a new random map.
    // targetFloorPercentage is how much floor space you want to be open (as a percentage)
    // maxTunnelLength is how long you want tunnels. 
    //--------------------------------------------------------------------------------------------
    void RandomMap(const float targetFloorPercentage, const int maxTunnelLength);

    // Get a new direction. Used by the RandomMap method.
    int GetNewDirection(const int currentRow, const int currentColumn, int currentDir) const;

    const int m_mapSize;
    const float m_tileWidth;
    const float m_tileHeight;
    std::array<std::array<EMapValue, MAP_SIZE>, MAP_SIZE> m_tileValues;
};
#endif