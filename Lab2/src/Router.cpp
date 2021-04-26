#include <iostream>
#include <cmath>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <functional>
#include <vector>
#include <algorithm>

#include "Router.h"

Router::Router()
{
    grid = new Grid();
}

void Router::start()
{
    sortNets();

    auto cmp = [&](Net *a, Net *b) {
        return (a->pinNum > b->pinNum);
    };
    priority_queue<Net *, std::vector<Net *>, decltype(cmp)> priorityQueue(cmp);
    for (auto net : nets)
    {
        priorityQueue.push(net);
    }

    while (!priorityQueue.empty())
    {
        auto net = priorityQueue.top();
        priorityQueue.pop();

        while (true)
        {
            auto result = astarRouting(net);
            if (result.first)
            {
                break;
            }

            cout << "Stuck at (" << result.second->pos.first << ", " << result.second->pos.second << ")" << endl;
            int id = grid->nearestNet(result.second->pos);
            auto demolishnet = nets[id];
            demolishNet(demolishnet);
            priorityQueue.push(demolishnet);
        }
    }
}

void Router::addNet(string name, int srcX, int srcY, int dstX, int dstY)
{
    auto net = new Net(name, nets.size(), srcX, srcY, dstX, dstY);
    nets.push_back(net);
    grid->setObstacle(srcX, srcY);
    grid->setObstacle(dstX, dstY);
}

void Router::buildNet(Net *net, std::vector<Grid::GridBox *> &path)
{
    for (auto box : path)
    {
        box->setPath(net->id);
        net->path.push_back(box->pos);
    }
    net->isRouted = true;
    cout << "Build net " << net->name << endl;
}

void Router::demolishNet(Net *net)
{
    for (auto pos : net->path)
    {
        grid->getGridbox(pos)->clearPath();
    }
    grid->getGridbox(net->src)->setPath(net->id);
    grid->getGridbox(net->dst)->setPath(net->id);

    net->path.clear();
    net->isRouted = false;
    cout << "Demolish net " << net->name << endl;
}

void Router::sortNets()
{
    for (auto net : nets)
    {
        net->calculatePinNum(nets);
        cout << *net << endl;
    }
    // std::sort(nets.begin(), nets.end(), [](Router::Net *a, Router::Net *b) { return a->pinNum < b->pinNum; });
}

pair<bool, Grid::GridBox *> Router::astarRouting(Net *net)
{
    Grid::GridBox *nearestGridbox = nullptr;
    // heuristic function
    auto heuristic = [&](Grid::GridBox *gridbox) {
        int x1, y1, x2, y2;
        x1 = gridbox->pos.first;
        y1 = gridbox->pos.second;
        x2 = net->dst.first;
        y2 = net->dst.second;

        return abs(x1 - x2) + abs(y1 - y2);
    };

    // reach list
    unordered_map<int, Grid::GridBox *> reachedGridboxes;

    // priorityQueue
    auto cmp = [&](Grid::GridBox *a, Grid::GridBox *b) {
        return ((double(a->cost) + heuristic(a)) > (double(b->cost) + heuristic(b)));
    };
    priority_queue<Grid::GridBox *, std::vector<Grid::GridBox *>, decltype(cmp)> priorityQueue(cmp);

    // Push the src into priority queue
    auto srcGridbox = grid->getGridbox(net->src);
    srcGridbox->cost = 0;
    srcGridbox->path.push_back(srcGridbox);
    priorityQueue.push(srcGridbox);

    // Set dst terminal to not obstacle
    grid->getGridbox(net->dst)->clearObstacle();

    int minDistance = INT_MAX;

    // start iteration
    while (priorityQueue.size() != 0)
    {
        auto gridbox = priorityQueue.top();
        priorityQueue.pop();

        if (gridbox->getPos() == net->dst)
        {
            cout << "Find path\n";
            buildNet(net, gridbox->path);
            cout << *grid;
            return pair<bool, Grid::GridBox *>(true, nearestGridbox);
        }

        if (heuristic(gridbox) < minDistance)
        {
            nearestGridbox = gridbox;
        }

        auto adjGridboxes = grid->getAdjGridboxes(gridbox);
        for (auto adjGridbox : adjGridboxes)
        {

            // check wether reached before
            int id = grid->posToId(adjGridbox->getPos());
            Grid::GridBox *reachGridbox = nullptr;
            auto findIter = reachedGridboxes.find(id);
            if (findIter != reachedGridboxes.end())
            {
                reachGridbox = findIter->second;
            }

            if (reachGridbox == nullptr || adjGridbox->cost < reachGridbox->cost)
            {
                adjGridbox->cost = gridbox->cost + 1;
                adjGridbox->path = gridbox->path;
                adjGridbox->path.push_back(adjGridbox);

                reachedGridboxes[id] = adjGridbox;
                priorityQueue.push(adjGridbox);
            }

            // cout << *adjGridbox << endl;
        }
    }

    cout << "Cannot find path\n";
    return pair<bool, Grid::GridBox *>(false, nearestGridbox);
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
    for (auto netPtr : R.nets)
    {
        out << netPtr->outputFormat();
    }
    return out;
}
