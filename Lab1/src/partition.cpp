#include <iostream>

#include "partition.h"

// BucketList member Function
Partition::BucketList::BucketList(){};

void Partition::BucketList::setPmax(int pinMax_in)
{
    pinMax = pinMax_in;
    for (int i = 0; i < 2 * pinMax + 1; i++)
    {
        std::vector<int> list;
        bucket.push_back(list);
    }
}

void Partition::BucketList::clear()
{
    int size = bucket.size();
    for (int i = 0; i < size; i++)
    {
        bucket[i].clear();
    }
    maxGain = INT_MIN;
}

void Partition::BucketList::insert(int gain, int nodeId)
{
    bucket[gain + pinMax].push_back(nodeId);
    if (gain + pinMax > maxGain)
    {
        maxGain = gain;
    }
}

void Partition::BucketList::print()
{
    std::cout << "\tmaxGain: " << maxGain << "\n";
    for (int i = bucket.size() - 1; i >= 0; i--)
    {
        auto list = bucket[i];
        std::cout << "\t" << (i - pinMax) << ":\t[ ";
        for (auto node : list)
        {
            std::cout << node << " ";
        }
        std::cout << "]\n";
    }
    std::cout << "\n";
}

// Partition Member functions
Partition::Partition(HyperGraph *graph) : graph(graph)
{
    for (int i = 0; i <= graph->nodeCount; i++)
    {
        // partition.push_back(i % 2);
        partition.push_back(i == 1 || i == 3 || i == 4 || i == 7);
    }
    leftBucket.setPmax(graph->pinMax);
    rightBucket.setPmax(graph->pinMax);
}

void Partition::printPartition()
{
    std::cout << "left:";

    for (int i = 1; i < int(partition.size()); i++)
    {
        if (partition[i])
        {
            std::cout << i << " ";
        }
    }
    std::cout << "\n";
    std::cout << "right: ";
    for (int i = 1; i < int(partition.size()); i++)
    {
        if (!partition[i])
        {
            std::cout << i << " ";
        }
    }
    std::cout << "\n";
}

void Partition::printBucket()
{
    std::cout << "left: ";
    leftBucket.print();

    std::cout << "right: ";
    rightBucket.print();

    std::cout << "\n";
}

int Partition::gain(int nodeId)
{
    auto edges = graph->getAdjEdges(nodeId);

    int FS = 0;
    int TE = 0;

    bool side = partition[nodeId];

    for (auto edge_ptr : edges)
    {
        auto nodeSet = edge_ptr->nodes;
        bool FS_flag = true;
        bool TE_flag = true;

        for (auto node : *nodeSet)
        {
            if (partition[node] != side)
            {
                TE_flag = false;
                break;
            }
        }
        for (auto node : *nodeSet)
        {
            if (partition[node] == side && node != nodeId)
            {
                FS_flag = false;
                break;
            }
        }

        if (FS_flag)
        {
            FS++;
        }
        else if (TE_flag)
        {
            TE++;
        }
    }

    return FS - TE;
}

void Partition::calculateGain()
{
    leftBucket.clear();
    rightBucket.clear();

    for (auto node : graph->nodes)
    {
        int nodeId = node.first;
        int nodegain = gain(nodeId);

        if (partition[nodeId])
        {
            leftBucket.insert(nodegain, nodeId);
        }
        else
        {
            rightBucket.insert(nodegain, nodeId);
        }
    }
}