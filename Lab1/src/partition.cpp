#include <iostream>
#include <algorithm>

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
    if(bucketPosList.find(nodeId) == bucketPosList.end()){
            std::cout<<"cannot find: "<<nodeId<<"\n";
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
        if(!list.size()){
            continue;
        }
        std::cout << "\t" << (i - pinMax) <<" size: "<<list.size()<< ":\t[ ";
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
    for (int i = 0; i <= graph->nodeCount; i++)
    {
        bool initialPartitionLogic = (i % 2);
        // bool initialPartitionLogic = (i == 1 || i == 3 || i == 4 || i == 7);

        partition.push_back(initialPartitionLogic);
        locklist.push_back(false);

        if (initialPartitionLogic)
        {
            leftCount++;
        }
        else
        {
            rightCount++;
        }
    }
    if (partition[0])
    {
        leftCount--;
    }
    else
    {
        rightCount--;
    }

    leftBucket.setPmax(graph->pinMax);
    rightBucket.setPmax(graph->pinMax);

    areaConstrain.first = graph->nodeCount * balanceFactor;
    areaConstrain.second = graph->nodeCount - areaConstrain.first;
    if (areaConstrain.first > areaConstrain.second)
    {
        std::swap(areaConstrain.first, areaConstrain.second);
    }

    std::cout<<"Area Constrain: "<<areaConstrain.first<<" "<<areaConstrain.second<<'\n';

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

void Partition::calculateInitialGain()
{
    leftBucket.clear();
    rightBucket.clear();

    for (auto node : graph->nodes)
    {
        int nodeId = node.first;
        int nodegain = gain(nodeId);

        if (partition[nodeId])
        {
            leftBucket.insert(nodeId, nodegain);
        }
        else
        {
            rightBucket.insert(nodeId, nodegain);
        }
    }
}

void Partition::calculateGain(std::set<int> nodes)
{
    for (auto node : nodes)
    {
        if (checkLock(node))
        {
            // std::cout<<node<<" is locked\n";
            continue;
        }
        int nodegain = gain(node);

        if (partition[node])
        {
            leftBucket.update(node, nodegain);
        }
        else
        {
            rightBucket.update(node, nodegain);
        }
    }
}

void Partition::changeSide(int nodeId)
{
    if (partition[nodeId])
    {
        leftCount--;
        rightCount++;
    }
    else
    {
        rightCount--;
        leftCount++;
    }
    partition[nodeId] = !partition[nodeId];
}

void Partition::lock(int nodeId)
{
    locklist[nodeId] = true;
    lockCount++;
}

bool Partition::checkBalance()
{
    return !(leftCount > areaConstrain.second || rightCount > areaConstrain.second);
}

bool Partition::checkLock(int nodeId)
{
    return locklist[nodeId];
}

bool Partition::checkAllLock()
{
    return lockCount == graph->nodeCount;
}

void Partition::FM_Algorithm()
{
    calculateInitialGain();
    // printBucket();
    // std::cout << leftBucket.maxGain << " " << rightBucket.maxGain << std::endl;

    int count = 0;
    while (!checkAllLock())
    {
        std::cout<<"\n------------loop "<<count<<"------------\n";
        auto max = getMaxGainNodeFromBucketlist();
        // printBucket();
        int maxNode = max.first;
        int gain = max.second;

        if (maxNode == -1)
        {
            std::cout<<"Bucket Empty\n";
            break;
        }

        std::cout << "Change and lock Node: " << maxNode << "\n";
        std::cout << "Gain: " << gain << "\n";
        std::cout << "GainSum: " << gainSum << "\n";
        std::cout << "BestGainSum: " << maxGainSum << "\n";



        changeSide(maxNode);
        lock(maxNode);

        gainSum += gain;
        if(gainSum > maxGainSum){
            maxGainSum = gainSum;
            maxPartition = partition;
        }

        auto adjNodes = graph->getAdjNodes(maxNode);

        calculateGain(adjNodes);

        // printBucket();

        std::cout<<"--------------------------------\n";

        count++;

    }

    std::cout<<"maxGainSum: "<<maxGainSum<<std::endl;
}

std::pair<int, int> Partition::getMaxGainNodeFromBucketlist()
{
    int leftMax = leftBucket.maxGain;
    int rightMax = rightBucket.maxGain;
    int maxGain;
    int maxNode;
    // std::cout << leftMax << " " << rightMax << "\n";
    while (true)
    {
        // std::cout << leftMax << " " << rightMax << "\n";
        if (leftMax + leftBucket.pinMax < 0 && rightMax + rightBucket.pinMax < 0)
        {
            return std::pair<int, int>(-1, 0);
        }
        
        if (leftMax > rightMax)
        {
            maxGain = leftMax;
            int maxGainIndex = maxGain + leftBucket.pinMax;
            if (maxGainIndex < 0)
            {
                continue;
            }
            for(auto node : leftBucket.bucket[maxGainIndex])
            {
                changeSide(node);
                if (checkBalance())
                {
                    // leftBucket.print();
                    // std::cout<<"Find max left node: "<<node<<" gain: "<<maxGain<<" size: "<<leftBucket.bucket[maxGainIndex].size()<<"\n";

                    changeSide(node);
                    leftBucket.remove(node);
                    return std::pair<int, int>(node, maxGain);
                }
                else
                {
                    changeSide(node);
                }
            }
            leftMax--;
        }
        else
        {
            maxGain = rightMax;
            int maxGainIndex = maxGain + rightBucket.pinMax;
            if (maxGainIndex < 0)
            {
                continue;
            }
            for(auto node : rightBucket.bucket[maxGainIndex])
            {
                changeSide(node);
                if (checkBalance())
                {
                    // rightBucket.print();
                    // std::cout<<"Find max right node: "<<node<<" gain: "<<maxGain<<" size: "<<rightBucket.bucket[maxGainIndex].size()<<"\n";
                    changeSide(node);
                    rightBucket.remove(node);
                    // std::cout<<" size: "<<rightBucket.bucket[maxGainIndex].size()<<"\n";
                    
                    return std::pair<int, int>(node, maxGain);
                }
                else
                {
                    changeSide(node);
                }
            }
            rightMax--;
        }
    }

    
}

std::ostream &operator<<(std::ostream &out, const Partition &P){
    int size = P.partition.size();
    for(int i = 1;i < size;i++){
        out<<P.maxPartition[i]<<'\n';
    }
    return out;
}