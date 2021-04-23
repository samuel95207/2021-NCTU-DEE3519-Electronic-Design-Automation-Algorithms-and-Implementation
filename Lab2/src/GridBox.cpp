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

void Grid::GridBox::setPath()
{
    obstacle = true;
    isPath = true;
}

void Grid::GridBox::setSymbol(string str){
    symbol = str;
}

void Grid::GridBox::setSymbol(char c){
    string str = "";
    str += c;
    symbol = str;
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
        if(symbol == ""){
            return ".";
        }else{
            return symbol;
        }
    }
    else if (obstacle)
    {
        return "X";
    }
    return " ";
}
