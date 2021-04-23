#include <cmath>
#include <iostream>
#include <sstream>

#include "Router.h"

using namespace std;

Router::Net::Net(string name, int srcX, int srcY, int dstX, int dstY) : name(name)
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
            oss << prevPos.first << " " << prevPos.second << endl << prevPos.first << " " << prevPos.second << " ";
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
