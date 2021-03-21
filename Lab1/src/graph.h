#include <string>
#include <map>
#include <utility>
#include <vector>
#include <set>

struct Edge
{
    int n1_id;
    int n2_id;
    double weight;
    bool directed;

    bool operator<(const Edge &a) const;
    bool operator>(const Edge &a) const;
    bool operator==(const Edge &a) const;

    Edge();
    Edge(int, int, int, bool);

    friend std::ostream &operator<<(std::ostream &out, const Edge &edge);
};

class Graph
{
    class Node
    {
        int id;
        std::map<int, Edge *> edges;
        bool directed;

    public:
        Node();
        Node(int id, bool directed);
        Edge *addEdge(int n2_id, double weight);

        std::vector<int> getAdjNodes();
        std::vector<Edge> getEdges();
        int getId();

        friend class Graph;
    };

    std::map<int, Node *> nodes;
    std::vector<Edge> edges;

    bool directed;

    int nodeNum;
    int edgeNum;
    double weightSum;

public:
    Graph();
    Graph(bool directed);

    void addNode(int id);
    void addEdge(int n1_id, int n2_id, double weight);
    bool removeNode(int id);
    bool removeEdge(int n1_id, int n2_id);

    std::vector<int> getNodes();
    std::vector<int> getAdjNodes(int id);

    std::vector<Edge> getEdges();
    Edge getEdge(int n1_id, int n2_id);
    double getEdgeWeight(int n1_id, int n2_id);
    std::vector<Edge> getAdjEdges(int id);
    bool isDirected();

    bool checkNode(int id);
    bool checkEdge(int n1_id, int n2_id);

    friend std::ostream &operator<<(std::ostream &out, const Graph &graph);

    // other graph algorithms
    Graph primeMST();
    Graph kruskalMST();

    std::pair<std::vector<Edge>* , double>* breakCycle();

private:
    std::vector<Edge> primeUndirectedBreakCycle();
    std::pair<std::vector<Edge>*, double>* kruskalUndirectedBreakCycle();

    std::pair<std::vector<Edge>*, double>* directedBreakCycle();

    bool DFS(Edge edge, int cutoff);
    bool BFS(Edge edge, int cutoff);
};