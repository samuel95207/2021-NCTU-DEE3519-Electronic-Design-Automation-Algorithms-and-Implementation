#include <iostream>
#include <algorithm>
#include <cstdlib>

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

void Partition::BucketList::insert(int nodeId, int gain)
{
    // std::cout<<"insert: "<<nodeId<<" gain: "<<gain<<"\n";
    bucket[gain + pinMax].push_back(nodeId);
    bucketPosList[nodeId] = std::pair<int, int>(gain + pinMax, bucket[gain + pinMax].size() - 1);

    if (gain > maxGain)
    {
        maxGain = gain;
    }
}

void Partition::BucketList::update(int nodeId, int gain)
{
    // std::cout<<"update: "<<nodeId<<"\n";
    remove(nodeId);
    insert(nodeId, gain);
}

void Partition::BucketList::remove(int nodeId)
{
    // std::cout<<"remove: "<<nodeId<<"\n";
    if (bucketPosList.find(nodeId) == bucketPosList.end())
    {
        std::cout << "cannot find: " << nodeId << "\n";
        return;
    }
    auto nodePos = bucketPosList[nodeId];
    bucketPosList.erase(nodeId);
    // std::cout<<"pos: "<<nodePos.first<<" "<<nodePos.second<<"\n";

    bucketPosList[bucket[nodePos.first][bucket[nodePos.first].size() - 1]] = nodePos;
    std::swap(bucket[nodePos.first][nodePos.second], bucket[nodePos.first][bucket[nodePos.first].size() - 1]);
    bucket[nodePos.first].pop_back();

    // for(auto node : bucket[nodePos.first]){
    //     std::cout<<node<<" ";
    // }
    // std::cout<<"\n";
    // std::cout<<"size "<<bucket[nodePos.first].size()<<"\n";

    if (nodePos.first == maxGain + pinMax)
    {
        while (maxGain + pinMax >= 0)
        {
            if (bucket[maxGain + pinMax].size())
            {
                break;
            }
            maxGain--;
        }
    }
}

void Partition::BucketList::print()
{
    std::cout << "\tmaxGain: " << maxGain << "\n";
    for (int i = bucket.size() - 1; i >= 0; i--)
    {
        auto list = bucket[i];
        if (!list.size())
        {
            continue;
        }
        std::cout << "\t" << (i - pinMax) << " size: " << list.size() << ":\t[ ";
        for (auto node : list)
        {
            std::cout << node << " ";
        }
        std::cout << "]\n";
    }
    std::cout << "\n";
}

// Partition Member functions
Partition::Partition(HyperGraph *graph, double balanceFactor) : graph(graph)
{
    partition.resize(graph->nodeCount + 1);
    locklist.resize(graph->nodeCount + 1);
    
    int methodCount = 3;
    int minMethod = 0;
    int minCutsize = INT_MAX;
    int cutsize;

    for(int method = 0;method < methodCount;method++){
        initialPartition(method);
        cutsize = calculateCutsize();
        if(cutsize < minCutsize){
            minMethod = method;
            minCutsize = cutsize;
        }
    }

    initialPartition(minMethod);
    cutsize = calculateCutsize();

    std::cout << "Initial cutsize: " << cutsize << '\n';

    leftBucket.setPmax(graph->pinMax);
    rightBucket.setPmax(graph->pinMax);

    areaConstrain.first = graph->nodeCount * balanceFactor;
    areaConstrain.second = graph->nodeCount - areaConstrain.first;
    if (areaConstrain.first > areaConstrain.second)
    {
        std::swap(areaConstrain.first, areaConstrain.second);
    }

    // std::cout << "Area Constrain: " << areaConstrain.first << " " << areaConstrain.second << '\n';

    maxPartition = partition;
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

std::ostream &operator<<(std::ostream &out, const Partition &P)
{
    int size = P.partition.size();
    for (int i = 1; i < size; i++)
    {
        out << P.maxPartition[i] << '\n';
    }
    return out;
}