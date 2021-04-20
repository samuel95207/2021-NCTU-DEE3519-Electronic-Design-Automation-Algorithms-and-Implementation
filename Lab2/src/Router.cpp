#include <iostream>
#include <sstream>

#include "Router.h"

Router::Router()
{
    grid = new Grid();
}

void Router::addNet(string name, int srcX, int srcY, int dstX, int dstY)
{
    nets.push_back(new Net(name, srcX, srcY, dstX, dstY));
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
