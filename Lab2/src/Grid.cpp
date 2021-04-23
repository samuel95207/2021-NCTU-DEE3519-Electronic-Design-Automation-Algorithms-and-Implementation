#include <iostream>
#include <sstream>
#include <string>

#include "Grid.h"

using namespace std;

Grid::Grid(int height, int width) : height(height), width(width)
{
    setSize(height, width);
}

void Grid::setSize(int height_in, int width_in)
{
    height = height_in;
    width = width_in;
    gridboxes.clear();
    for (int y = 0; y < height; y++)
    {
        vector<GridBox> row;
        for (int x = 0; x < width; x++)
        {
            row.push_back(GridBox(x, y, false));
        }
        gridboxes.push_back(row);
    }
}

void Grid::addBlock(int leftDownX, int leftDownY, int rightUpX, int rightUpY)
{
    for (int y = leftDownY; y <= rightUpY; y++)
    {
        for (int x = leftDownX; x <= rightUpX; x++)
        {
            gridboxes[y][x].setObstacle();
        }
    }
}

void Grid::setObstacle(int x, int y)
{
    gridboxes[y][x].setObstacle();
}

void Grid::setPath(int x, int y)
{
    gridboxes[y][x].setPath();
}

Grid::GridBox *Grid::getGridbox(int x, int y)
{
    return &gridboxes[y][x];
}

Grid::GridBox *Grid::getGridbox(pair<int, int> pos)
{
    return &gridboxes[pos.second][pos.first];
}

vector<Grid::GridBox *> Grid::getAdjGridboxes(Grid::GridBox *gridbox)
{
    auto pos = gridbox->getPos();
    int x = pos.first;
    int y = pos.second;

    vector<pair<int, int>> adjPosList;
    adjPosList.push_back(pair<int, int>(x + 1, y));
    adjPosList.push_back(pair<int, int>(x - 1, y));
    adjPosList.push_back(pair<int, int>(x, y + 1));
    adjPosList.push_back(pair<int, int>(x, y - 1));

    vector<Grid::GridBox *> adjList;

    for (auto adjPos : adjPosList)
    {
        if (!isInBound(adjPos))
        {
            continue;
        }
        auto gridbox = getGridbox(adjPos);
        if (!gridbox->isObstacle())
        {
            adjList.push_back(gridbox);
        }
    }
    return adjList;
}

bool Grid::isInBound(pair<int, int> pos)
{
    return pos.first >= 0 && pos.first < width && pos.second >= 0 && pos.second < height;
}

int Grid::posToId(int x, int y)
{
    return width * y + x;
}

int Grid::posToId(pair<int, int> pos){
    return posToId(pos.first, pos.second);
}


pair<int, int> Grid::idToPos(int id)
{
    int y = id / width;
    int x = id % width;
    return pair<int, int>(x, y);
}

std::ostream &operator<<(std::ostream &out, const Grid &G)
{
    for (int i = 0; i < G.width + 1; i++)
    {
        out << "---";
    }
    out << endl;
    for (int y = G.height - 1; y >= 0; y--)
    {
        out << "|";
        for (int x = 0; x < G.width; x++)
        {
            out << " " << G.gridboxes[y][x].toString() << " ";
        }
        out << "|\n";
    }
    for (int i = 0; i < G.width + 1; i++)
    {
        out << "---";
    }
    out << endl;
    return out;
}
