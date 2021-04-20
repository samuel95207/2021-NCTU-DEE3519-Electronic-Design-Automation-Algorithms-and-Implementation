#include <cmath>
#include <iostream>

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

int Router::Net::calculateArea()
{
    area = (abs(dst.first - src.first) + 1) * (abs(dst.second - src.second) + 1);
    return area;
}

