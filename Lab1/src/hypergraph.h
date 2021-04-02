#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <set>

class Partition;

struct Edge
{

    std::set<int> *nodes;
    double weight;

    bool operator<(const Edge &a) const;
    bool operator>(const Edge &a) const;
    bool operator==(const Edge &a) const;

    Edge();
    Edge(std::set<int> *, double);

    friend std::ostream &operator<<(std::ostream &out, const Edge &edge);
};

class HyperGraph
{
    class Node
    {
        int id;
        // std::unordered_map<int, Edge *> edges;
        std::vector<Edge *> edgeList;
        std::set<int> adjNodeList;

    public:
        Node();
        Node(int id);
        Edge *addEdge(Edge *edge);

        std::set<int> *getAdjNodes();
        std::vector<Edge *> getEdges();
        int getId();

        friend class HyperGraph;
    };

    std::unordered_map<int, Node *> nodes;
    std::vector<Edge *> edges;

    int nodeCount;
    int edgeCount;
    int pinSum = 0;
    int pinMax = 0;
    double weightSum;

public:
    HyperGraph();

    void addNode(int id);
    void addEdge(std::set<int> *, double weight);

    std::vector<int> getNodes();
    std::set<int> *getAdjNodes(int id);

    std::vector<Edge *> getEdges();
    // Edge getEdge(int n1_id, int n2_id);
    // double getEdgeWeight(int n1_id, int n2_id);
    std::vector<Edge *> getAdjEdges(int id);

    friend std::ostream &operator<<(std::ostream &out, const HyperGraph &graph);

    friend class Partition;
};