#include <vector>
#include <climits>
#include <utility>

using namespace std;

class Grid
{
    struct GridBox;

    int height;
    int width;

    vector<vector<GridBox>> gridboxes;

public:
    Grid(){};
    Grid(int height, int width);

    void setSize(int height, int width);
    void addBlock(int leftDownX, int leftDownY, int rightUpX, int rightUpY);
    void setObstacle(int x, int y);
    void setPath(int x, int y);

    friend std::ostream &operator<<(std::ostream &out, const Grid &G);

    friend class Router;
};

struct Grid::GridBox
{
    bool obstacle = false;
    bool path = false;

    pair<int, int> pos;
    int cost = INT_MAX;

    GridBox() {}
    GridBox(int x, int y, bool obstacle);

    void setObstacle();
    void clearObstacle();
    void setPath();
    
    bool isObstacle() const;
    string toString() const;

    friend std::ostream &operator<<(std::ostream &out, const GridBox &G)
    {
        out << "pos: (" << G.pos.first << ", " << G.pos.second << ")  cost:" << G.cost << endl;
        return out;
    }
};
