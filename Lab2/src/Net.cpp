#include <cmath>
#include <iostream>
#include <sstream>

#include "Router.h"

using namespace std;

Router::Net::Net(string name, int id, int srcX, int srcY, int dstX, int dstY) : name(name), id(id)
{
    src.first = srcX;
    src.second = srcY;
    dst.first = dstX;
    dst.second = dstY;

    calculateArea();
}

string Router::Net::outputFormat()
{
    ostringstream oss;
    if (path.empty())
    {
        oss << " ";
        return oss.str();
    }
    oss << name << endl;
    oss << "begin" << endl;
    oss << path.size() - 2 << endl;

    auto cornerPos = path[0];
    auto prevPos = path[0];

    oss << prevPos.first << " " << prevPos.second << " ";

    for (auto pos : path)
    {
        if (pos.first != cornerPos.first && pos.second != cornerPos.second)
        {
            oss << prevPos.first << " " << prevPos.second << endl
                << prevPos.first << " " << prevPos.second << " ";
            cornerPos = prevPos;
        }
        prevPos = pos;

        // oss<<"prevpos "<<prevPos.first<<" "<<prevPos.second<<endl;
    }

    oss << prevPos.first << " " << prevPos.second << endl;

    oss << "end" << endl;

    return oss.str();
}

int Router::Net::calculateArea()
{
    area = (abs(dst.first - src.first) + 1) * (abs(dst.second - src.second) + 1);
    return area;
}

int Router::Net::calculatePinNum(vector<Net *> nets)
{
    pinNum = 0;
    int xMax = max(src.first, dst.first);
    int xMin = min(src.first, dst.first);
    int yMax = max(src.second, dst.second);
    int yMin = min(src.second, dst.second);

    for (auto net : nets)
    {
        if (net->src.first <= xMax && net->src.first >= xMin && net->src.second <= yMax && net->src.second >= yMin)
        {
            pinNum++;
        }
        if (net->dst.first <= xMax && net->dst.first >= xMin && net->dst.second <= yMax && net->dst.second >= yMin)
        {
            pinNum++;
        }
    }
    return pinNum;
}
