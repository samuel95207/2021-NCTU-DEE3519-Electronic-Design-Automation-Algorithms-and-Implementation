#include <hypergraph.h>
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
Edge::Edge(std::set<int> *nodes, double weight) : nodes(nodes), weight(weight) {}

Edge::Edge() {}

bool Edge::operator<(const Edge &a) const
{
    return weight < a.weight;
}

bool Edge::operator==(const Edge &a) const
{
    return (nodes == a.nodes) && (weight == a.weight);
}

bool Edge::operator>(const Edge &a) const
{
    return weight > a.weight;
}

std::ostream &operator<<(std::ostream &out, const Edge &edge)
{
    std::cout << "{ ";
    for (auto node : *(edge.nodes))
    {
        std::cout << node << " ";
    }
    std::cout << "} w:" << edge.weight;
    return out;
}

// Member functions for HyperGraph::Node
HyperGraph::Node::Node()
{
}

HyperGraph::Node::Node(int id) : id(id)
{
}

std::vector<int> HyperGraph::Node::getAdjNodes()
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

std::vector<Edge> HyperGraph::Node::getEdges()
{
    std::vector<Edge> result;
    for (auto iter = edges.begin(); iter != edges.end(); iter++)
    {
        auto edge = *(iter->second);
        if (find(result.begin(), result.end(), edge) == result.end())
        {
            result.push_back(edge);
        }
    }
    return result;
}

int HyperGraph::Node::getId()
{
    return id;
}

Edge *HyperGraph::Node::addEdge(Edge *edge)
{

    for (auto node : *(edge->nodes))
    {
        if (node == id)
        {
            continue;
        }
        edges[node] = edge;
        // std::cout<<id<<" add edge "<<*edge<<" to "<<node<<std::endl;
    }
    return edge;
}

//Member functions for HyperGraph
HyperGraph::HyperGraph()
{
    nodeCount = 0;
    edgeCount = 0;
}

void HyperGraph::addNode(int id)
{
    auto node = new Node(id);
    nodes[id] = node;
    nodeCount++;
}

void HyperGraph::addEdge(std::set<int>* nodes_in, double weight = 1)
{
    Edge *edge = new Edge(nodes_in, weight);
    for (auto node : *(edge->nodes))
    {
        auto node_iter = nodes.find(node);
        if (node_iter == nodes.end())
        {
            nodes[node] = new Node(node);
        }
        nodes[node]->addEdge(edge);
    }

    edges.push_back(edge);

    edgeCount += 1;
    weightSum += weight;
}

std::vector<int> HyperGraph::getNodes()
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

std::vector<int> HyperGraph::getAdjNodes(int id)
{
    return nodes[id]->getAdjNodes();
}

std::vector<Edge> HyperGraph::getEdges()
{
    std::vector<Edge> result;
    result.resize(edges.size());
    int index = 0;
    for (auto iter = edges.begin(); iter != edges.end(); iter++)
    {
        result[index] = **(iter);
        index++;
    }
    return result;
}

Edge HyperGraph::getEdge(int n1_id, int n2_id)
{
    return *(nodes[n1_id]->edges[n2_id]);
}

double HyperGraph::getEdgeWeight(int n1_id, int n2_id)
{
    return getEdge(n1_id, n2_id).weight;
}

std::vector<Edge> HyperGraph::getAdjEdges(int id)
{
    return nodes[id]->getEdges();
}

std::ostream &operator<<(std::ostream &out, const HyperGraph &graph)
{
    auto nodes = graph.nodes;

    for (auto iter = nodes.begin(); iter != nodes.end(); iter++)
    {
        auto node = iter->second;
        auto edges = node->getEdges();

        out << node->getId() << ": ";
        for (auto edge : edges)
        {
            out << edge << " ";
        }
        out << "\n";
    }

    out << "\n";
    return out;
}
