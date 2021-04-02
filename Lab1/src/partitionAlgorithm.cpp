#include <iostream>
#include <algorithm>
#include <cstdlib>

#include "partition.h"

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
            if(node == nodeId){
                continue;
            }
            if(!TE_flag  && !FS_flag){
                break;
            }
            if (partition[node] != side)
            {
                TE_flag = false;
               
            }
            if (partition[node] == side && node != nodeId)
            {
                FS_flag = false;
            }
        }
        // for (auto node : *nodeSet)
        // {
        //     if(node == nodeId){
        //         continue;
        //     }
        //     if (partition[node] == side && node != nodeId)
        //     {
        //         FS_flag = false;
        //         break;
        //     }
        // }

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

void Partition::reset(){
    leftBucket.clear();
    rightBucket.clear();
    lockCount = 0;
    gainSum = 0;
    maxGainSum = INT_MIN;
    leftCount = maxPartitionLeftCount;
    rightCount = maxPartitionRightCount;

    for (int i = 0; i <= graph->nodeCount; i++)
    {
        locklist[i] = false;
    }
}

void Partition::FM_Algorithm()
{
    calculateInitialGain();
    // printBucket();
    // std::cout << leftBucket.maxGain << " " << rightBucket.maxGain << std::endl;

    int count = 0;
    while (!checkAllLock())
    {
        
        auto max = getMaxGainNodeFromBucketlist();
        int maxNode = max.first;
        int gain = max.second;

        if (maxNode == -1)
        {
            std::cout << "Bucket Empty\n";
            break;
        }


        changeSide(maxNode);
        lock(maxNode);

        gainSum += gain;
        if (gainSum > maxGainSum)
        {
            maxGainSum = gainSum;
            maxPartition = partition;
            maxPartitionLeftCount = leftCount;
            maxPartitionRightCount = rightCount;
        }

        auto adjNodes = graph->getAdjNodes(maxNode);

        calculateGain(adjNodes);


        // std::cout << "\n------------loop " << count << "------------\n";
        // std::cout << "Change and lock Node: " << maxNode << "\n";
        // std::cout << "Gain: " << gain << "\n";
        // std::cout << "lockCount: " << lockCount << "\n";
        // std::cout << "GainSum: " << gainSum << "\n";
        // std::cout << "BestGainSum: " << maxGainSum << "\n";
        // std::cout << "--------------------------------\n";

        

        count++;
    }

    std::cout << "maxGainSum: " << maxGainSum << std::endl;
}

void Partition::iterate(int count){
    for(int i = 0;i < count; i++){
        partition = maxPartition;
        FM_Algorithm();
        reset();
    }
}

std::pair<int, int> Partition::getMaxGainNodeFromBucketlist()
{
    int leftMax = leftBucket.maxGain;
    int rightMax = rightBucket.maxGain;
    int maxGain;
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
            for (auto node : leftBucket.bucket[maxGainIndex])
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
            for (auto node : rightBucket.bucket[maxGainIndex])
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