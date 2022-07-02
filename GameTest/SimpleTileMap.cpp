//------------------------------------------------------------------------
// Quick and dirty tile map class.
// Feel free to use this for your entry if you want but you don't have to.
// If you do not use this then you should provide an alternative that represents a pac-man style map.
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
#include <assert.h>  
//------------------------------------------------------------------------
#include "app\app.h"
#include "SimpleTileMap.h"

// Color mapping for bird's eye view render. Indexed based on tile value.
const CTile CTile::tiles[NUM_TILE_TYPES] = {
	//CTile{ 0.0f, 0.0f, 1.0f, 10.0f, 1.0f },		//Border
	//CTile{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },		//Air
	//CTile{ 0.0f, 0.0f, 0.5f, 10.0f, 1.0f },		//Wall
	//CTile{ 0.8f, 0.8f, 0.0f, 2.5f, 0.25f }      //Coin (unused)
    CTile{ 0.0f, 0.0f, 1.0f, 10.0f, 1.0f },		//Border
    CTile{ 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },		//Air
    CTile{ 0.0f, 0.0f, 0.0f, 10.0f, 1.0f },		//Wall
    CTile{ 0.8f, 0.8f, 0.0f, 2.5f, 0.25f }      //Coin (unused)
};

// Direction lookup used by random map generation.
static int g_dirLookup[4][2] =
{
    { -1, 0 },	// Left
    {  1, 0 },	// Up
    {  0,-1 },	// Down
    {  0, 1 }	// Right
};

void CSimpleTileMap::Create()
{    
    m_tileValues.clear();
    m_tileValues.resize(m_mapSize);
    for (int i = 0; i < m_mapSize; i++)
        m_tileValues[i].resize(m_mapSize, EMapValue::WALL);

    m_tileWidth = (float)APP_VIRTUAL_WIDTH / (float)m_mapSize;
    m_tileHeight = (float)APP_VIRTUAL_HEIGHT / (float)m_mapSize;
}

void CSimpleTileMap::Clear(EMapValue clearValue)
{    
    for (auto &row : m_tileValues)
    {
        row.assign(row.size(), clearValue);     
    }
}

bool CSimpleTileMap::SetTileMapValue(const int x, const int y, const EMapValue v)
{
    if ((x >= 0 && x < m_mapSize) && (y >= 0 && y < m_mapSize))
    {
        m_tileValues[x][y] = v;
        return true;
    }
    return false;
}

EMapValue CSimpleTileMap::GetTileMapValue(const int x, const int y) const
{
    if ((x >= 0 && x < m_mapSize) && (y >= 0 && y < m_mapSize))
    {
        return m_tileValues[x][y];
    }
    return EMapValue::OUTOFBOUNDS;
}

EMapValue CSimpleTileMap::GetTileMapValue(const float fx, const float fy) const
{
    int x = (int)(fx / m_tileWidth);
    int y = (int)(fy / m_tileHeight);
    return GetTileMapValue(x, y);
}

Cell CSimpleTileMap::GetCell(float x, float y) const
{
	return { int(x / m_tileWidth), int(y / m_tileHeight) };
}

Cell CSimpleTileMap::GetCell(const CPoint& point) const
{
	return GetCell(point.x, point.y);
}

void CSimpleTileMap::DrawTile(const Cell& cell, float r, float g, float b) const
{
	App::DrawQuad(cell.x * GetTileWidth(), cell.y * GetTileHeight(), (cell.x + 1) * GetTileWidth(), (cell.y + 1) * GetTileHeight(), r, g, b);
}

void CSimpleTileMap::Render() const
{
    const float xStep = m_tileWidth;
    const float yStep = m_tileHeight;
    for (int y = 0; y < m_mapSize; y++)
    {
        for (int x = 0; x < m_mapSize; x++)
        {
			EMapValue index = GetTileMapValue(x, y);
			if (index > EMapValue::OUTOFBOUNDS && index < EMapValue::NUM_TILE_TYPES) {
				const CTile& tile = CTile::tiles[index];

				float xPos = (x * xStep);
				xPos += (xStep - (xStep * tile.scale)) / 2.0f;

				float yPos = (y * yStep);
				yPos += (yStep - (yStep * tile.scale)) / 2.0f;

				float w = xStep * tile.scale;
				float h = yStep * tile.scale;

				App::DrawQuad(xPos, yPos, xPos + w, yPos + h, tile.r, tile.g, tile.b);
			}
#if _DEBUG
			else {
				printf("Tried to render an invalid tile of value %i at row %i column %i.\n", index, y, x);
				printf("Press enter to terminate the program.\n");
				getchar();
				exit(0);
			}
#endif
        }
    }
}

float CSimpleTileMap::GetTileWidth() const
{
	return m_tileWidth;
}

float CSimpleTileMap::GetTileHeight() const
{
	return m_tileHeight;
}

CSimpleTileMap::CSimpleTileMap(const int mapSize) : m_tileCount(mapSize * mapSize), m_mapSize(mapSize)
{
	Create();
}

CPoint CSimpleTileMap::Ricochet(const CPoint& position, const CPoint& translation) const
{
    CPoint destination{ position + translation };
    EMapValue xTile = GetTileMapValue(destination.x, position.y);
    EMapValue yTile = GetTileMapValue(position.x, destination.y);
    if (yTile == EMapValue::BORDER || yTile == EMapValue::WALL || yTile == EMapValue::OUTOFBOUNDS)
        destination.y -= translation.y;
    if (xTile == EMapValue::BORDER || xTile == EMapValue::WALL || xTile == EMapValue::OUTOFBOUNDS)
        destination.x -= translation.x;
    return destination;
}

/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////PATHING BEGIN/////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

Path CSimpleTileMap::FindPath(const MCell& start, const MCell& end)
{
    // Reset nodes, mark all nodes as unvisited (closed list = false) and append start to open list
    m_tileNodes.clear();
    m_tileNodes.resize(m_tileCount);
    m_tileNodes[GetCellIndex(start)].parentCell = start;
    m_closedList.resize(m_tileCount, false);
    m_openList.push({ start });

    while (!m_openList.empty())
    {
        const MCell currentCell = m_openList.top().cell;

        // End condition (destination reached)
        if (currentCell == end)
        {
            break;
        }

        // Otherwise, add current cell to closed list and update g & h values of its neighbours
        m_openList.pop();
        m_closedList[GetCellIndex(currentCell)] = true;

        int gNew, hNew;
        for (const MCell& neighbour: GetNeighbours(currentCell))
        {
            const int neighbourIndex = GetCellIndex(neighbour);

            // Skip if already visited
            if (m_closedList[neighbourIndex])
                continue;

            auto manhattan = [](const MCell& a, const MCell& b) -> int {
                return abs(a.x - b.x) + abs(a.y - b.y);
            };

            auto euclidean = [](const MCell& a, const MCell& b) -> int {
                int dx = a.x - b.x;
                int dy = a.y - b.y;
                return (int)sqrt(dx * dx + dy * dy);
            };

            // Calculate scores
            gNew = m_tileNodes[GetCellIndex(currentCell)].g + 1;
            hNew = false ? manhattan(neighbour, end) : euclidean(neighbour, end);

            // Append if unvisited or best score
            if (m_tileNodes[neighbourIndex].f() == 0 || (gNew + hNew) < m_tileNodes[neighbourIndex].f())
            {
                m_openList.push({ neighbour, gNew, hNew });
                m_tileNodes[neighbourIndex] = { neighbour, currentCell, gNew, hNew};
            }
        }
    }

    // Generate path by traversing parents then inverting
    Path path;
    MCell currentCell = end;
    int currentIndex = GetCellIndex(currentCell);

    while (!(m_tileNodes[currentIndex].parentCell == currentCell))
    {
        path.push_back(currentCell);
        currentCell = m_tileNodes[currentIndex].parentCell;
        currentIndex = GetCellIndex(currentCell);
    }
    std::reverse(path.begin(), path.end());

    return path;
}

std::vector<MCell> CSimpleTileMap::GetNeighbours(const MCell& cell) const
{
    std::vector<MCell> cells;
    for (int i = cell.x - 1; i <= cell.x + 1 && i >= 0 && i < GetMapSize(); i++)
    {
        for (int j = cell.y - 1; j <= cell.y + 1 && j >= 0 && j < GetMapSize(); j++)
        {
            if (!(i == cell.x && j == cell.y) && GetTileMapValue(i, j) == EMapValue::AIR)
                cells.push_back({ i, j });
        }
    }
    return cells;
}

void CSimpleTileMap::DrawPath(const Path& path) const
{
    assert(path.size() > 1);
    for (const MCell& cell : path)
        DrawTile(cell.ToCell(), 1.0f, 0.0f, 0.0f);

    DrawTile(path.front().ToCell(), 0.5f, 0.0f, 0.0f);
    DrawTile(path.back().ToCell(), 0.0f, 0.5f, 0.0f);
}

int CSimpleTileMap::GetCellIndex(const MCell& cell) const
{
    return cell.y * m_mapSize + cell.x;
}
/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////PATHING END///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Randomly creates tunnels through the map.
// Picks a direction then moves in a random direction of length (0-maxTunnelLength)
// Picks new direction and repeats until we have filled the map with the targetFloorPercentage of FLOOR tiles.
//------------------------------------------------------------------------
void CSimpleTileMap::RandomMap(const float targetFloorPercentage, const int maxTunnelLength)
{
    // Clear the map as WALLs then fill the BORDERs.
    Clear();

    float percentageOfFloorCovered = 0.0f;
    float oneTilesPercentage = 100.0f / (float)(m_mapSize * m_mapSize);

    // Set the borders.
    for (int y = 0; y < m_mapSize; y++)
    {
        SetTileMapValue(0, y, EMapValue::BORDER);
        SetTileMapValue(m_mapSize-1, y, EMapValue::BORDER); 
        percentageOfFloorCovered += oneTilesPercentage*2;
    }
    for (int x = 1; x < m_mapSize-1; x++)
    {
        SetTileMapValue(x, 0, EMapValue::BORDER);
        SetTileMapValue(x, m_mapSize - 1, EMapValue::BORDER);
        percentageOfFloorCovered += oneTilesPercentage * 2;
    }    
    int lastDir = rand() % 4;               // Last direction we filled the map in.
    int currentRow = 1 + (rand() % (m_mapSize-2));
    int currentColumn = 1 + (rand() % (m_mapSize-2));
          
    while( percentageOfFloorCovered < targetFloorPercentage )
    {
        int currentDir = GetNewDirection(currentRow, currentColumn, lastDir);         // Get a new random direction.
        int randomLength = 1 + rand() % (maxTunnelLength-1); //length the next tunnel will be (max of maxLength)
        for (int l = 0; l < randomLength; l++)
        {               
            // If the next step will take you into a BORDER of the grid then change direction.
            if (GetTileMapValue(currentRow + g_dirLookup[currentDir][0], currentColumn + g_dirLookup[currentDir][1]) == EMapValue::BORDER)
            {
                currentDir = GetNewDirection(currentRow, currentColumn, currentDir);
            }
            lastDir = currentDir;
            currentRow += g_dirLookup[currentDir][0];
            currentColumn += g_dirLookup[currentDir][1];
            if (GetTileMapValue(currentRow, currentColumn) != EMapValue::AIR)
            {
                percentageOfFloorCovered += oneTilesPercentage;
            }

            SetTileMapValue(currentRow, currentColumn, EMapValue::AIR);
        }        
    }
}

int CSimpleTileMap::GetNewDirection(const int currentRow, const int currentColumn, int currentDir) const
{
    // Get random direction. Perp to the last direction.
    int newDir = rand() % 2;
    if (currentDir < 2)
        newDir += 2;
    //If the new tile direction would hit a border then go in the opposite direction.
    if (GetTileMapValue(currentRow + g_dirLookup[newDir][0], currentColumn + g_dirLookup[newDir][1]) == EMapValue::BORDER)
    {
        switch (newDir)
        {
        case 0:
            return 1;
        case 1:
            return 0;
        case 2:
            return 3;
        case 3:
            return 2;
        }
    }
    return newDir;
}
