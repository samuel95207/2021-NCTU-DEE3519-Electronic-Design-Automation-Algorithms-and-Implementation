#include <vector>
#include <utility>

#include "Grid.h"

using namespace std;

class Router
{
    Grid *grid;

public:
    Router();
    Router(Grid *G) : grid(G){};

    friend std::istream &operator>>(std::istream &in, Router &R);
    friend std::ostream &operator<<(std::ostream &out, const Router R);
};