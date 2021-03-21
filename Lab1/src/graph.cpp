#include <graph.h>
#include <iostream>
#include <utility>
#include <set>
#include <algorithm>
#include <string>
#include <stack>
#include <queue>
#include <unionSet.h>
#include <ctime>

// Member function for Edge
Edge::Edge(int n1_id, int n2_id, int weight, bool directed = false) : n1_id(n1_id), n2_id(n2_id), weight(weight), directed(directed) {}

Edge::Edge() {}

bool Edge::operator<(const Edge &a) const
{
    return weight > a.weight;
}

bool Edge::operator==(const Edge &a) const
{
    if (directed != a.directed)
    {
        return false;
    }
    if (directed)
    {
        return (n1_id == a.n1_id) && (n2_id == a.n2_id);
    }
    else
    {
        return ((n1_id == a.n1_id) && (n2_id == a.n2_id)) || ((n1_id == a.n2_id) && (n2_id == a.n1_id));
    }
}

bool Edge::operator>(const Edge &a) const
{
    return weight < a.weight;
}

std::ostream &operator<<(std::ostream &out, const Edge &edge)
{
    out << "{" << edge.n1_id;
    if (edge.directed)
    {
        out << "}--w=" << edge.weight << "->{";
    }
    else
    {
        out << "}--w=" << edge.weight << "--{";
    }
    out << edge.n2_id << "}";
    return out;
}

// Member functions for Graph::Node
Graph::Node::Node()
{
}

Graph::Node::Node(int id, bool directed = false) : id(id), directed(directed)
{
}

Edge *Graph::Node::addEdge(int n2_id, double weight = 0)
{
    auto edge = new Edge;
    edge->n1_id = id;
    edge->n2_id = n2_id;
    edge->weight = weight;
    edge->directed = directed;
    edges[n2_id] = edge;
    return edge;
}

std::vector<int> Graph::Node::getAdjNodes()
{
    std::vector<int> result;
    result.resize(edges.size());
    int index = 0;
    for (auto iter = edges.begin(); iter != edges.end(); iter++)
    {
        result[index] = iter->first;
        index++;
    }
    return result;
}

std::vector<Edge> Graph::Node::getEdges()
{
    std::vector<Edge> result;
    result.resize(edges.size());
    int index = 0;
    for (auto iter = edges.begin(); iter != edges.end(); iter++)
    {
        result[index] = *(iter->second);
        index++;
    }
    return result;
}

int Graph::Node::getId()
{
    return id;
}

//Member functions for Graph
Graph::Graph()
{
    nodeNum = 0;
    edgeNum = 0;
    directed = false;
}

Graph::Graph(bool is_directed)
{
    nodeNum = 0;
    edgeNum = 0;
    directed = is_directed;
}

void Graph::addNode(int id)
{
    auto node = new Node(id, directed);
    nodes[id] = node;
    nodeNum++;
}

void Graph::addEdge(int n1_id, int n2_id, double weight = 0)
{
    auto n1_iter = nodes.find(n1_id);
    auto n2_iter = nodes.find(n2_id);

    if (n1_iter == nodes.end())
    {
        addNode(n1_id);
        n1_iter = nodes.find(n1_id);
    }
    if (n2_iter == nodes.end())
    {
        addNode(n2_id);
        n2_iter = nodes.find(n2_id);
    }

    auto n1 = n1_iter->second;
    auto n2 = n2_iter->second;

    n1->addEdge(n2_id, weight);
    edges.push_back(Edge(n1_id, n2_id, weight));

    if (!directed)
    {
        n2->addEdge(n1_id, weight);
    }

    edgeNum += 1;
    weightSum += weight;
}

bool Graph::removeNode(int id)
{
    auto iter = nodes.find(id);
    if (iter == nodes.end())
    {
        return false;
    }
    nodes.erase(iter);

    for (iter = nodes.begin(); iter != nodes.end(); iter++)
    {
        auto edges = &(iter->second->edges);
        auto edge = edges->find(id);
        if (edge != edges->end())
        {
            edges->erase(edge);
        }
    }
    nodeNum--;
    return true;
}

bool Graph::removeEdge(int n1_id, int n2_id)
{

    auto n1_iter = nodes.find(n1_id);

    if (n1_iter == nodes.end())
    {
        return false;
    }

    auto n1 = n1_iter->second;

    auto edge = n1->edges.find(n2_id);

    if (edge == n1->edges.end())
    {
        return false;
    }

    n1->edges.erase(edge);

    if (!directed)
    {
        auto n2_iter = nodes.find(n2_id);
        // if (n2_iter == nodes.end())
        // {
        //     return false;
        // }
        auto n2 = n2_iter->second;
        n2->edges.erase(n2->edges.find(n1_id));
    }
    edges.erase(find(edges.begin(), edges.end(), *(edge->second)));
    edgeNum--;
    return true;
}

std::vector<int> Graph::getNodes()
{
    std::vector<int> result;
    result.resize(nodes.size());
    int index = 0;
    for (auto iter = nodes.begin(); iter != nodes.end(); iter++)
    {
        result[index] = iter->first;
        index++;
    }
    return result;
}

std::vector<int> Graph::getAdjNodes(int id)
{
    return nodes[id]->getAdjNodes();
}

std::vector<Edge> Graph::getEdges()
{
    std::vector<Edge> result;
    result.assign(edges.begin(), edges.end());
    return result;
}

Edge Graph::getEdge(int n1_id, int n2_id)
{
    return *(nodes[n1_id]->edges[n2_id]);
}

double Graph::getEdgeWeight(int n1_id, int n2_id)
{
    return getEdge(n1_id, n2_id).weight;
}

std::vector<Edge> Graph::getAdjEdges(int id)
{
    return nodes[id]->getEdges();
}

bool Graph::isDirected()
{
    return directed;
}

bool Graph::checkNode(int id)
{
    return !(nodes.find(id) == nodes.end());
}

bool Graph::checkEdge(int n1_id, int n2_id)
{
    auto node = *(nodes.find(n1_id)->second);
    auto edges = node.edges;
    return !(edges.find(n2_id) == edges.end());
}

std::ostream &operator<<(std::ostream &out, const Graph &graph)
{
    auto nodes = graph.nodes;

    for (auto iter = nodes.begin(); iter != nodes.end(); iter++)
    {
        auto node = iter->second;
        auto edges = node->getEdges();

        out << node->getId() << ": ";
        for (auto j_iter = edges.begin(); j_iter != edges.end(); j_iter++)
        {
            auto edge = *j_iter;
            out << edge << " ";
        }
        out << "\n";
    }

    out << "\n";
    return out;
}

Graph Graph::primeMST()
{
    Graph MST(directed);

    auto firstnode = getNodes()[0];
    MST.addNode(firstnode);

    while (MST.nodeNum != nodeNum)
    {
        std::set<Edge> connectEdges;
        auto MSTnodes = MST.getNodes();
        for (auto iter = MSTnodes.begin(); iter != MSTnodes.end(); iter++)
        {
            auto edges = getAdjEdges(*iter);
            for (auto edge_iter = edges.begin(); edge_iter != edges.end(); edge_iter++)
            {
                auto edge = *(edge_iter);
                if (MST.checkNode(edge.n1_id) && MST.checkNode(edge.n2_id))
                {
                    continue;
                }
                connectEdges.insert(edge);
            }
        }
        if (connectEdges.empty())
        {
            break;
        }

        // Print Process
        // for (auto iter = connectEdges.begin(); iter != connectEdges.end(); iter++)
        // {
        //     cout << *iter << " ";
        // }
        // cout << endl;

        auto edge = *connectEdges.begin();
        MST.addEdge(edge.n1_id, edge.n2_id, edge.weight);
        connectEdges.erase(edge);
    }

    return MST;
}

Graph Graph::kruskalMST()
{
    Graph MST(directed);
    int mst_wt = 0;

    sort(edges.begin(), edges.end());

    UnionSet us(nodeNum);

    for (auto iter = edges.begin(); iter != edges.end(); iter++)
    {
        auto edge = *iter;
        int n1 = edge.n1_id;
        int n2 = edge.n2_id;
        int weight = edge.weight;
        int set_n1 = us.find(n1);
        int set_n2 = us.find(n2);

        if (set_n1 != set_n2)
        {
            us.Union(set_n1, set_n2);
            MST.addEdge(n1, n2, weight);
        }
        else
        {
            // std::cout << n1 << " - " << n2 << " " << weight << std::endl;
            mst_wt += weight;
        }
    }
    std::cout << mst_wt << std::endl;
    return MST;
}

std::pair<std::vector<Edge> *, double>* Graph::breakCycle()
{
    if (!directed)
    {
        return kruskalUndirectedBreakCycle();
    }
    else
    {
        return directedBreakCycle();
    }
}

std::vector<Edge> Graph::primeUndirectedBreakCycle()
{
    Graph MST;
    // std::set<Edge> deletedEdges;
    std::vector<Edge> deletedEdges = getEdges();

    auto firstnode = getNodes()[0];
    MST.addNode(firstnode);

    while (MST.nodeNum != nodeNum)
    {
        std::set<Edge> connectEdges;
        // std::vector<Edge> connectEdges;

        auto MSTnodes = MST.getNodes();
        for (auto iter = MSTnodes.begin(); iter != MSTnodes.end(); iter++)
        {
            auto edges = getAdjEdges(*iter);
            for (auto edge_iter = edges.begin(); edge_iter != edges.end(); edge_iter++)
            {
                auto edge = *(edge_iter);
                if (MST.checkNode(edge.n1_id) && MST.checkNode(edge.n2_id))
                {
                    continue;
                }

                if (find(connectEdges.begin(), connectEdges.end(), edge) == connectEdges.end())
                {
                    connectEdges.insert(edge);
                    // connectEdges.push_back(edge);
                    // sort(connectEdges.begin(), connectEdges.end());
                }
            }
        }
        if (connectEdges.empty())
        {
            break;
        }

        // Print Process
        // for (auto iter = connectEdges.begin(); iter != connectEdges.end(); iter++)
        // {
        //     cout << *iter << " ";
        // }
        // cout << endl;

        auto edge = *connectEdges.begin();
        MST.addEdge(edge.n1_id, edge.n2_id, edge.weight);
        // connectEdges.erase(find(connectEdges.begin(), connectEdges.end(), edge));
        connectEdges.erase(edge);

        auto iter = find(deletedEdges.begin(), deletedEdges.end(), edge);
        deletedEdges.erase(iter);
    }

    return deletedEdges;
}

std::pair<std::vector<Edge>*, double>* Graph::kruskalUndirectedBreakCycle()
{
    int weightSum = 0;
    auto deletedEdges = new std::vector<Edge>();

    sort(edges.begin(), edges.end());

    UnionSet us(nodeNum);

    for (auto iter = edges.begin(); iter != edges.end(); iter++)
    {
        auto edge = *iter;
        int n1 = edge.n1_id;
        int n2 = edge.n2_id;
        int weight = edge.weight;
        int set_n1 = us.find(n1);
        int set_n2 = us.find(n2);

        if (set_n1 != set_n2)
        {
            us.Union(set_n1, set_n2);
        }
        else
        {
            deletedEdges->push_back(edge);
            weightSum += weight;
        }
    }
    return new std::pair<std::vector<Edge>*, double>(deletedEdges, weightSum);
}

std::pair<std::vector<Edge>*, double>* Graph::directedBreakCycle()
{
    auto c1 = std::clock();

    Graph MST(directed);

    std::vector<Edge> deletedEdges;

    sort(edges.begin(), edges.end());

    auto c2 = std::clock();

    UnionSet us(nodeNum+1000000);

    for (auto iter = edges.begin(); iter != edges.end(); iter++)
    {
        int weightSum = 0;
        auto edge = *iter;
        int n1 = edge.n1_id;
        int n2 = edge.n2_id;
        int weight = edge.weight;
        int set_n1 = us.find(n1);
        int set_n2 = us.find(n2);

        if (set_n1 != set_n2)
        {
            us.Union(set_n1, set_n2);
            MST.addEdge(n1, n2, weight);
        }
        else
        {
            deletedEdges.push_back(edge);
            weightSum += edge.weight;
        }
    }

    auto c3 = std::clock();

    auto deletedEdgesPhase2 = new std::vector<Edge>();

    weightSum = 0;
    int count = 0;

    for (auto i = deletedEdges.begin(); i != deletedEdges.end(); i++)
    {
        count++;
        // std::cout<<count<<std::endl;
        auto edge = *i;
        if (edge.weight < 0)
        {
            deletedEdgesPhase2->push_back(*i);
            weightSum += edge.weight;
        }
        else if (MST.BFS(edge, MST.edgeNum * 250))
        {
            deletedEdgesPhase2->push_back(*i);
            weightSum += edge.weight;
        }
        else
        {
            MST.addEdge(edge.n1_id, edge.n2_id, edge.weight);
        }
    }

    auto c4 = std::clock();

    std::cout << "sort time: " << double(c2 - c1) / CLOCKS_PER_SEC << std::endl;
    std::cout << "MST  time: " << double(c3 - c2) / CLOCKS_PER_SEC << std::endl;
    std::cout << "DFS  time: " << double(c4 - c3) / CLOCKS_PER_SEC << std::endl;

    return new std::pair<std::vector<Edge>*, double>(deletedEdgesPhase2, weightSum);
}

bool Graph::DFS(Edge edge, int cutoff)
{
    std::stack<int> S;

    int n1 = edge.n1_id;
    int n2 = edge.n2_id;

    int count = 0;

    S.push(n2);

    while (!S.empty())
    {
        count++;
        int n = S.top();
        S.pop();

        // std::cout<<n<<std::endl;

        if (n == n1 || count > cutoff)
        {
            return true;
        }

        auto edges = &(nodes[n]->edges);
        for (auto iter = edges->begin(); iter != edges->end(); iter++)
        {
            S.push(iter->first);
        }
    }
    return false;
}

bool Graph::BFS(Edge edge, int cutoff)
{
    std::queue<int> Q;

    int n1 = edge.n1_id;
    int n2 = edge.n2_id;

    int count = 0;

    Q.push(n2);

    while (!Q.empty())
    {
        count++;
        int n = Q.front();
        Q.pop();

        // std::cout<<n<<std::endl;

        if (n == n1 || count > cutoff)
        {
            return true;
        }

        auto edges = &(nodes[n]->edges);
        for (auto iter = edges->begin(); iter != edges->end(); iter++)
        {
            Q.push(iter->first);
        }
    }
    return false;
}