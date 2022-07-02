#include "Pathing.h"
#include <queue>

namespace Pathing {
    struct Node
    {
        Node()
        {
            init();
        }

        Node(const Cell& cell)
        {
            init(cell);
        }

        Node(const Cell& cell, int g, int h)
        {
            init(cell, { -1, -1 }, g, h);
        }

        Node(const Cell& cell, const Cell& parentCell, int g, int h)
        {
            init(cell, parentCell, g, h);
        }

        void init(const Cell& cell = { -1, -1 }, const Cell& parentCell = { -1, -1 }, int g = 0, int h = 0)
        {
            this->cell = cell;
            this->parentCell = parentCell;
            this->g = g;
            this->h = h;
        }

        int f() const { return g + h; }

        void Print()
        {
            printf("Cell {%i,%i}: f = %i (%ig + %ih)\n", cell.x, cell.y, f(), g, h);
        }

        Cell cell, parentCell;
        int g, h;
    };

    CPoint FollowPath(const CPoint& start, const CPoint& end, float speed, const CSimpleTileMap& map)
    {
        return CPoint();
    }

    Path FindPath(const Cell& start, const Cell& end, const CSimpleTileMap& map)
    {
        // computationally cheap but treats diagonals the same as adjacents
        auto manhattan = [](const Cell& a, const Cell& b) -> int {
            return abs(a.x - b.x) + abs(a.y - b.y);
        };

        // computationally expensive but treats diagonals as more expensive than adjacents
        auto euclidean = [](const Cell& a, const Cell& b) -> int {
            int dx = a.x - b.x;
            int dy = a.y - b.y;
            return (int)sqrt(dx * dx + dy * dy);
        };

        // priority_queue orders its elements *GREATEST* to least, but we want its elements
        // least-to-greatest in order to obtain the best rather than the worst path!
        auto predicate = [](const Node& a, const Node& b) -> bool { return a.f() > b.f(); };

        // Mark all nodes as unvisited (closed list = false) and append start to open list
        const int tileCount = map.GetMapSize() * map.GetMapSize();
        std::vector<Node> tileNodes(tileCount);
        std::priority_queue<Node, std::vector<Node>, decltype(predicate)> openList(predicate);
        std::vector<bool> closedList(tileCount, false);
        tileNodes[GetCellIndex(start, map)].parentCell = start;
        openList.push({ start });

        while (!openList.empty())
        {
            const Cell currentCell = openList.top().cell;

            // End condition (destination reached)
            if (currentCell == end)
            {
                break;
            }

            // Otherwise, add current cell to closed list and update g & h values of its neighbours
            openList.pop();
            closedList[GetCellIndex(currentCell, map)] = true;

            int gNew, hNew;
            for (const Cell& neighbour : GetNeighbours(currentCell, map))
            {
                const int neighbourIndex = GetCellIndex(neighbour, map);

                // Skip if already visited
                if (closedList[neighbourIndex])
                    continue;

                // Calculate scores
                gNew = tileNodes[GetCellIndex(currentCell, map)].g + 1;
                hNew = false ? manhattan(neighbour, end) : euclidean(neighbour, end);

                // Append if unvisited or best score
                if (tileNodes[neighbourIndex].f() == 0 || (gNew + hNew) < tileNodes[neighbourIndex].f())
                {
                    openList.push({ neighbour, gNew, hNew });
                    tileNodes[neighbourIndex] = { neighbour, currentCell, gNew, hNew };
                }
            }
        }

        // Generate path by traversing parents then inverting
        Path path;
        Cell currentCell = end;
        int currentIndex = GetCellIndex(currentCell, map);

        while (!(tileNodes[currentIndex].parentCell == currentCell))
        {
            path.push_back(currentCell);
            currentCell = tileNodes[currentIndex].parentCell;
            currentIndex = GetCellIndex(currentCell, map);
        }
        std::reverse(path.begin(), path.end());

        return path;
    }

    void DrawPath(const Path& path, const CSimpleTileMap& map)
    {
        if (path.size() > 1)
        {
            for (const Cell& cell : path)
                map.DrawTile(cell, 1.0f, 0.0f, 0.0f);

            map.DrawTile(path.front(), 0.5f, 0.0f, 0.0f);
            map.DrawTile(path.back(), 0.0f, 0.5f, 0.0f);
        }
    }

    CPoint Ricochet(const CPoint& position, const CPoint& translation, const CSimpleTileMap& map)
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

    std::vector<Cell> GetNeighbours(const Cell& cell, const CSimpleTileMap& map)
    {
        std::vector<Cell> cells;
        for (int i = cell.x - 1; i <= cell.x + 1 && i >= 0 && i < map.GetMapSize(); i++)
        {
            for (int j = cell.y - 1; j <= cell.y + 1 && j >= 0 && j < map.GetMapSize(); j++)
            {
                if (!(i == cell.x && j == cell.y) && map.GetTileMapValue(i, j) == EMapValue::AIR)
                    cells.push_back({ i, j });
            }
        }
        return cells;
    }

    int GetCellIndex(const Cell& cell, const CSimpleTileMap& map)
    {
        return cell.y * map.GetMapSize() + cell.x;
    }
}
