#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <algorithm>

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
    gridboxes = new GridBox *[height];
    for (int y = 0; y < height; y++)
    {
        auto row = new GridBox[width];
        for (int x = 0; x < width; x++)
        {
            row[x] = GridBox(x, y, false);
        }
        gridboxes[y] = row;
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

void Grid::setPath(int x, int y, int netId)
{
    gridboxes[y][x].setPath(netId);
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

int Grid::posToId(pair<int, int> pos)
{
    return posToId(pos.first, pos.second);
}

pair<int, int> Grid::idToPos(int id)
{
    int y = id / width;
    int x = id % width;
    return pair<int, int>(x, y);
}

int Grid::nearestNet(pair<int, int> pos)
{
    auto up = pos;
    auto down = pos;
    auto left = pos;
    auto right = pos;

    vector<int> order = {0, 1, 2, 3};
    random_shuffle(order.begin(), order.end());

    while (true)
    {
        // cout<<"up "<<up.first<<" "<<up.second<<endl;
        // cout<<"down "<<down.first<<" "<<down.second<<endl;
        // cout<<"left "<<left.first<<" "<<left.second<<endl;
        // cout<<"right "<<right.first<<" "<<right.second<<endl<<endl;

        bool outOfBound = true;

        int random = rand() % 3;

        for (auto i : order)
        {
            if (i == 0)
            {
                if (isInBound(up))
                {
                    outOfBound = false;
                    if (gridboxes[up.second][up.first].isPath && !random)
                    {
                        return gridboxes[up.second][up.first].netId;
                    }
                    else
                    {
                        up.second += 1;
                    }
                }
            }
            else if (i == 1)
            {
                if (isInBound(down))
                {
                    outOfBound = false;
                    if (gridboxes[down.second][down.first].isPath && !random)
                    {
                        return gridboxes[down.second][down.first].netId;
                    }
                    else
                    {
                        down.second -= 1;
                    }
                }
            }
            else if (i == 2)
            {
                if (isInBound(left))
                {
                    outOfBound = false;
                    if (gridboxes[left.second][left.first].isPath && !random)
                    {
                        return gridboxes[left.second][left.first].netId;
                    }
                    else
                    {
                        left.first -= 1;
                    }
                }
            }
            else if (i == 3)
            {
                if (isInBound(right))
                {
                    outOfBound = false;
                    if (gridboxes[right.second][right.first].isPath && !random)
                    {
                        return gridboxes[right.second][right.first].netId;
                    }
                    else
                    {
                        right.first += 1;
                    }
                }
            }
        }

        if (outOfBound)
        {
            return -1;
        }
    }
}

std::ostream &operator<<(std::ostream &out, const Grid &G)
{
    if (G.width > 50 || G.height > 50)
    {
        return out;
    }
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
        out << "|" << y << "\n";
    }
    for (int i = 0; i < G.width + 1; i++)
    {
        out << "---";
    }
    out << "\n ";
    for (int i = 0; i < G.width; i++)
    {
        out << " " << i;
        if (i < 10)
        {
            out << " ";
        }
    }
    out << endl;
    return out;
}
