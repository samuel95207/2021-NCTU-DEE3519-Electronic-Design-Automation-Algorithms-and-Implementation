#include <iostream>
#include <cmath>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <functional>
#include <vector>

#include "Router.h"

Router::Router()
{
    grid = new Grid();
}

void Router::start()
{
    astarRouting(nets[0]);
}

void Router::addNet(string name, int srcX, int srcY, int dstX, int dstY)
{
    nets.push_back(new Net(name, srcX, srcY, dstX, dstY));
    grid->setPath(srcX, srcY);
    grid->setPath(dstX, dstY);
}

void Router::astarRouting(Net *net)
{
    auto heuristic = [&](Grid::GridBox *gridbox) {
        int x1, y1, x2, y2;
        x1 = gridbox->pos.first;
        y1 = gridbox->pos.second;
        x2 = net->dst.first;
        y2 = net->dst.second;

        return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    };

    unordered_map<int, Grid::GridBox> reachedGrids;
    auto cmp = [&](Grid::GridBox *a, Grid::GridBox *b) {
        return ((double(a->cost) + heuristic(a)) > (double(b->cost) + heuristic(b)));
    };
    priority_queue<Grid::GridBox *, std::vector<Grid::GridBox *>, decltype(cmp)> priorityQueue(cmp);

    grid->gridboxes[7][4].cost = 0;
    grid->gridboxes[8][4].cost = 0;
    grid->gridboxes[9][4].cost = 0;
    grid->gridboxes[10][4].cost = 0;

    priorityQueue.push(&(grid->gridboxes[9][4]));
    priorityQueue.push(&(grid->gridboxes[7][4]));
    priorityQueue.push(&(grid->gridboxes[10][4]));
    priorityQueue.push(&(grid->gridboxes[8][4]));

    cout << *priorityQueue.top() << endl;
    priorityQueue.pop();
    cout << *priorityQueue.top() << endl;
    priorityQueue.pop();
    cout << *priorityQueue.top() << endl;
    priorityQueue.pop();
    cout << *priorityQueue.top() << endl;
    priorityQueue.pop();
}

std::istream &operator>>(std::istream &in, Router &R)
{
    string line;
    istringstream iss;

    int col, row, blockNum, netNum;

    while (getline(in, line))
    {
        iss.clear();
        iss.str(line);

        string selection;
        iss >> selection;

        if (selection == ".row")
        {
            iss >> row;
        }
        else if (selection == ".col")
        {
            iss >> col;
        }
        else if (selection == ".blk")
        {
            R.grid->setSize(row, col);
            iss >> blockNum;
            for (int i = 0; i < blockNum; i++)
            {
                getline(in, line);

                iss.clear();
                iss.str(line);

                int leftDownX, leftDownY, rightUpX, rightUpY;
                iss >> leftDownX >> leftDownY >> rightUpX >> rightUpY;
                R.grid->addBlock(leftDownX, leftDownY, rightUpX, rightUpY);
            }
        }
        else if (selection == ".net")
        {
            iss >> netNum;
            for (int i = 0; i < netNum; i++)
            {
                getline(in, line);

                iss.clear();
                iss.str(line);

                string name;
                int srcX, srcY, dstX, dstY;
                iss >> name >> srcX >> srcY >> dstX >> dstY;
                R.addNet(name, srcX, srcY, dstX, dstY);
            }
        }
    }

    return in;
}

std::ostream &operator<<(std::ostream &out, const Router R)
{
    out << *(R.grid);
    for (auto netPtr : R.nets)
    {
        out << *netPtr;
    }
    return out;
}
