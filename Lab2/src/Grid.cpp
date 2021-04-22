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
 
void Grid::setObstacle(int x, int y){
    gridboxes[y][x].setObstacle();
}

void Grid::setPath(int x, int y){
    gridboxes[y][x].setPath();
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
            out << " " << G.gridboxes[y][x].toString() <<" ";
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
