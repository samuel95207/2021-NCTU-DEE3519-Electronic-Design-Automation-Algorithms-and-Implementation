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

void Grid::GridBox::clearObstacle(){
    obstacle = false;
}

void Grid::GridBox::setPath(){
    obstacle = true;
    path = true;
}

bool Grid::GridBox::isObstacle() const
{
    return obstacle;
}

string Grid::GridBox::toString() const
{
    if(path){
        return ".";
    }else if(obstacle){
        return "X";
    }
    return " ";
}
