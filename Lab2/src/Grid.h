#include <vector>
#include <utility>

using namespace std;

class Grid
{
    struct GridBox;

    int height;
    int width;

    vector<vector<GridBox>> gridbox;

public:
    Grid(){};
    Grid(int height, int width);

    void setSize(int height, int width);
    void addBlock(int leftDownX, int leftDownY, int rightUpX, int rightUpY);

    friend std::ostream &operator<<(std::ostream &out, const Grid G);
};

struct Grid::GridBox
{
    bool obstacle = false;
    pair<int, int> pos;

    GridBox() {}
    GridBox(int x, int y, bool obstacle);

    void setObstacle();
    bool isObstacle() const;
};
