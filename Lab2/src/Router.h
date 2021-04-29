#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#include "Grid.h"

using namespace std;

class Router
{
    struct Net;

    Grid *grid;
    vector<Net *> nets;

public:
    Router();
    Router(Grid *G) : grid(G){};

    void start();

    void addNet(string name, int srcX, int srcY, int dstX, int dstY);

    void buildNet(Net *net, Grid::GridBox * dst);
    bool demolishNet(Net *net);

    void sortNets();

    pair<bool, Grid::GridBox *> astarRouting(Net *net);

    friend std::istream &operator>>(std::istream &in, Router &R);
    friend std::ostream &operator<<(std::ostream &out, const Router R);
};

struct Router::Net
{
    string name;
    int id;
    pair<int, int> src;
    pair<int, int> dst;
    int area;
    int pinNum;

    bool isRouted;
    vector<pair<int, int>> path;

    Net(string name, int id, int srcX, int srcY, int dstX, int dstY);
    int calculateArea();
    int calculatePinNum(vector<Net *> nets);

    string outputFormat();

    friend std::ostream &operator<<(std::ostream &out, const Router::Net &N)
    {
        out << "name: " << N.name << endl;
        out << "id: " << N.id << endl;
        out << "src: (" << N.src.first << " " << N.src.second << ")\n";
        out << "dst: (" << N.dst.first << " " << N.dst.second << ")\n";
        out << "area: " << N.area << endl;
        out << "pinNum: " << N.pinNum << endl;
        return out;
    }
};
