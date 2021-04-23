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

    // Setters
    void setSize(int height, int width);
    void addBlock(int leftDownX, int leftDownY, int rightUpX, int rightUpY);
    void setObstacle(int x, int y);
    void setPath(int x, int y);

    // Getters
    GridBox *getGridbox(int x, int y);
    GridBox *getGridbox(pair<int, int>);
    vector<GridBox *> getAdjGridboxes(GridBox *gridbox);

    // Checker
    bool isInBound(pair<int, int> pos);

    // Utils
    int posToId(int x, int y);
    int posToId(pair<int, int> pos);
    pair<int, int> idToPos(int id);

    // IO
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
    pair<int, int> getPos();
    string toString() const;

    friend std::ostream &operator<<(std::ostream &out, const GridBox &G)
    {
        out << "pos: (" << G.pos.first << ", " << G.pos.second << ")  cost:" << G.cost;
        return out;
    }
};
