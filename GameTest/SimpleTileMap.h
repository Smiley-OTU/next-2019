#ifndef _SIMPLE_TILE_MAP_H
#define _SIMPLE_TILE_MAP_H
//------------------------------------------------------------------------
// Quick and dirty tile map class.
// Feel free to use this for your entry if you want but you don't have to.
// If you do not use this then you should provide an alternative that represents a pac-man style map.
//------------------------------------------------------------------------
#include <vector>
#include <utility>

using Cell = std::pair<int, int>;

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
    //--------------------------------------------------------------------------------------------
    // Constructor just creates a tile map of mapSize x mapSize.
    //--------------------------------------------------------------------------------------------
	CSimpleTileMap(const int mapSize);
	CSimpleTileMap();
	~CSimpleTileMap();

    //--------------------------------------------------------------------------------------------
    // This will generate a new random map.
    // targetFloorPercentage is how much floor space you want to be open (as a percentage)
    // maxTunnelLength is how long you want tunnels. 
    //--------------------------------------------------------------------------------------------
    // Just randomly creates tunnels through the map.
    // Picks a direction then moves in a random direction of length (0-maxTunnelLength)
    // Pick new direction and repeat until we have filled the map with the targetFloorPercentage of FLOOR tiles.
    //--------------------------------------------------------------------------------------------
    void RandomMap(const float targetFloorPercentage, const int maxTunnelLength);    
    //--------------------------------------------------------------------------------------------
    // Clear the map to give value.
    //--------------------------------------------------------------------------------------------
    void Clear(EMapValue clearValue = EMapValue::WALL );
    //--------------------------------------------------------------------------------------------
    // Get the tile value at give coords in grid.
    //--------------------------------------------------------------------------------------------
    EMapValue GetTileMapValue(const int x, const int y)  const;
    EMapValue GetTileMapValue(const float fx, const float fy) const;
	Cell GetCell(float x, float y);
	Cell GetCell(const CPoint& point);
    //--------------------------------------------------------------------------------------------
    // Set the tile value at give coords in grid.
    //--------------------------------------------------------------------------------------------
    bool SetTileMapValue(const int x, const int y, EMapValue v);
    //--------------------------------------------------------------------------------------------
    // Render the tile map using quads. Fills the entire screen.
    //--------------------------------------------------------------------------------------------
    void Render() const;
    // Return size of the map.
    int GetMapSize() const { return m_mapSize; }

	float getTileWidth() const;
	float getTileHeight() const;

private:
    // Create a new map.
    void Create();
    // Get a new direction. Used by the RandomMap method.
    int GetNewDirection(const int currentRow, const int currentColumn, int currentDir) const;
    int m_mapSize;                                  // Keep map size for convenience.
    float m_tileWidth;
    float m_tileHeight;
    std::vector<std::vector<EMapValue>> m_tileMap;  //Vector of vectors of map values, Holds the tile map data.
};
#endif