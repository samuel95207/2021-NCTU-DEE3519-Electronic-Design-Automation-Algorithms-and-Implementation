#include <iostream>
#include <sstream>
#include <string>

#include "Grid.h"

using namespace std;

Grid::GridBox::GridBox(int x, int y, bool obstacle) : obstacle(obstacle)
{
    pos.first = x;
    pos.second = y;
}

void Grid::GridBox::setObstacle()
{
    obstacle = true;
}

void Grid::GridBox::clearObstacle()
{
    obstacle = false;
}

void Grid::GridBox::setPath(int netId_in)
{
    obstacle = true;
    isPath = true;
    netId = netId_in;
}

void Grid::GridBox::clearPath(){
    obstacle = false;
    isPath = false;
    netId = -1;
}


bool Grid::GridBox::isObstacle() const
{
    return obstacle;
}

pair<int, int> Grid::GridBox::getPos()
{
    return pos;
}

string Grid::GridBox::toString() const
{
    if (isPath)
    {
        return to_string(netId);
    }
    else if (obstacle)
    {
        return "X";
    }
    return " ";
}
