#ifndef _SIMPLE_TILE_MAP_H
#define _SIMPLE_TILE_MAP_H
//------------------------------------------------------------------------
// Quick and dirty tile map class.
// Feel free to use this for your entry if you want but you don't have to.
// If you do not use this then you should provide an alternative that represents a pac-man style map.
//------------------------------------------------------------------------
#include "Cell.h"
#include "MutCell.h"
#include <vector>

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

    //Returns the collision response given a point and its desired translation.
    CPoint Ricochet(const CPoint& position, const CPoint& translation) const;

    //Returns a vector of tile grid indices to traverse to get from start to end (using A*).
    std::vector<MCell> FindPath(const MCell& start, const MCell& end);

    //Returns adjacent cells which can be moved to (tiles who's value is EMapValue::AIR).
    std::vector<MCell> GetNeighbours(const MCell& cell) const;

    void DrawPath(const std::vector<MCell>& path) const;

    int GetCellIndex(const MCell& cell) const;

    //--------------------------------------------------------------------------------------------
    // This will generate a new random map.
    // targetFloorPercentage is how much floor space you want to be open (as a percentage)
    // maxTunnelLength is how long you want tunnels. 
    //--------------------------------------------------------------------------------------------
    // Just randomly creates tunnels through the map.
    // Picks a direction then moves in a random direction of length (0-maxTunnelLength)
    // Pick new direction and repeat until we have filled the map with the targetFloorPercentage of FLOOR tiles.
    // 
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
	Cell GetCell(float x, float y) const;
	Cell GetCell(const CPoint& point) const;
	void DrawTile(const Cell& cell, float r, float g, float b) const;

    //--------------------------------------------------------------------------------------------
    // Set the tile value at give coords in grid.
    //--------------------------------------------------------------------------------------------
    bool SetTileMapValue(const int x, const int y, EMapValue v);

    //--------------------------------------------------------------------------------------------
    // Render the tile map using quads. Fills the entire screen.
    //--------------------------------------------------------------------------------------------
    void Render() const;

    // Returns the number of tiles per row/column (ie dimensions of 16x16 returns 16).
    int GetMapSize() const { return m_mapSize; }
	float GetTileWidth() const;
	float GetTileHeight() const;

private:
    // Create a new map.
    void Create();

    // Get a new direction. Used by the RandomMap method.
    int GetNewDirection(const int currentRow, const int currentColumn, int currentDir) const;

    const int m_tileCount;
    const int m_mapSize;
    float m_tileWidth;
    float m_tileHeight;
    std::vector<std::vector<EMapValue>> m_tileValues;

    struct Node
    {
        Node()
        {
            init();
        }

        Node(const MCell& cell)
        {
            init(cell);
        }

        Node(const MCell& cell, int g, int h)
        {
            init(cell, { -1, -1 }, g, h);
        }

        Node(const MCell& cell, const MCell& parentCell, int g, int h)
        {
            init(cell, parentCell, g, h);
        }

        void init(const MCell& cell = { -1, -1 }, const MCell& parentCell = { -1, -1 }, int g = 0, int h = 0)
        {
            this->cell = cell;
            this->parentCell = parentCell;
            this->g = g;
            this->h = h;
        }

        int f() const { return g + h; }

        bool operator<(const Node& node) const { return f() < node.f(); }

        MCell cell, parentCell;
        int g, h;
    };

    std::vector<Node> m_tileNodes;
    std::priority_queue<Node> m_openList;
    std::vector<bool> m_closedList;
};
#endif