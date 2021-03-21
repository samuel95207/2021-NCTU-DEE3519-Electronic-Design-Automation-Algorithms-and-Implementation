#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <set>

struct Edge
{

    std::set<int> nodes;
    double weight;

    bool operator<(const Edge &a) const;
    bool operator>(const Edge &a) const;
    bool operator==(const Edge &a) const;

    Edge();
    Edge(std::set<int> &, double);

    friend std::ostream &operator<<(std::ostream &out, const Edge &edge);
};

class HyperGraph
{
    class Node
    {
        int id;
        std::unordered_map<int, Edge *> edges;

    public:
        Node();
        Node(int id);
        Edge *addEdge(Edge *edge);

        std::vector<int> getAdjNodes();
        std::vector<Edge> getEdges();
        int getId();

        friend class HyperGraph;
    };

    std::unordered_map<int, Node *> nodes;
    std::vector<Edge *> edges;

    int nodeNum;
    int edgeNum;
    double weightSum;

public:
    HyperGraph();
    HyperGraph(bool directed);

    void addNode(int id);
    void addEdge(std::set<int>, double weight);

    std::vector<int> getNodes();
    std::vector<int> getAdjNodes(int id);

    std::vector<Edge> getEdges();
    Edge getEdge(int n1_id, int n2_id);
    double getEdgeWeight(int n1_id, int n2_id);
    std::vector<Edge> getAdjEdges(int id);

    friend std::ostream &operator<<(std::ostream &out, const HyperGraph &graph);
};