#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

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
            if (node == nodeId)
            {
                continue;
            }
            if (!TE_flag && !FS_flag)
            {
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

void Partition::calculateGain(std::set<int> *nodes)
{
    // std::cout<<"AdjNode count: "<< nodes->size()<<"\n";
    int checkTimeSum = 0;
    int gainTimeSum = 0;
    int updateTimeSum = 0;

    for (auto node : *nodes)
    {

        auto starttime = clock();

        if (checkLock(node))
        {
            // std::cout<<node<<" is locked\n";
            continue;
        }

        auto checktime = clock();

        int nodegain = gain(node);

        auto gaintime = clock();

        if (partition[node])
        {
            leftBucket.update(node, nodegain);
        }
        else
        {
            rightBucket.update(node, nodegain);
        }

        auto updatetime = clock();

        checkTimeSum += checktime-starttime;
        gainTimeSum += gaintime-checktime;
        updateTimeSum += updatetime-gaintime;
    }

    if(nodes->size() > 1000){
    std::cout << "check time: " << checkTimeSum / (double)CLOCKS_PER_SEC << std::endl;
    std::cout << "gain time: " << gainTimeSum / (double)CLOCKS_PER_SEC << std::endl;
    std::cout << "update time: " << updateTimeSum / (double)CLOCKS_PER_SEC << std::endl<<std::endl;
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

void Partition::reset()
{
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
    double GetMaxGainNodeFromBucketlist_timesum = 0;
    double ChangeSideAndLock_timesum = 0;
    double CopyMaxPartition_timesum = 0;
    double GetAdjNodes_timesum = 0;
    double CalculateGain_timesum = 0;
    double Total_timesum = 0;

    while (!checkAllLock())
    {
        // std::cout << "------------loop " << count << "------------\n";

        auto timestart = clock();

        auto max = getMaxGainNodeFromBucketlist();

        auto timeGetMaxGainNodeFromBucketlist = std::clock();

        int maxNode = max.first;
        int gain = max.second;

        if (maxNode == -1)
        {
            std::cout << "Bucket Empty\n";
            break;
        }

        changeSide(maxNode);
        lock(maxNode);

        auto timeChangeSideAndLock = std::clock();

        gainSum += gain;
        if (gainSum > maxGainSum)
        {
            maxGainSum = gainSum;
            maxPartition = partition;
            maxPartitionLeftCount = leftCount;
            maxPartitionRightCount = rightCount;
        }

        auto timeCopyMaxPartition = std::clock();

        auto adjNodes = graph->getAdjNodes(maxNode);

        auto timeGetAdjNodes = std::clock();

        calculateGain(adjNodes);

        auto timeCalculateGain = std::clock();

        // std::cout << "Change and lock Node: " << maxNode << "\n";
        // std::cout << "Gain: " << gain << "\n";
        // std::cout << "lockCount: " << lockCount << "\n";
        // std::cout << "GainSum: " << gainSum << "\n";
        // std::cout << "BestGainSum: " << maxGainSum << "\n";
        // std::cout << "GetMaxGainNodeFromBucketlist time: " << (timeGetMaxGainNodeFromBucketlist - timestart) / (double)CLOCKS_PER_SEC << std::endl;
        // std::cout << "ChangeSideAndLock time: " << (timeChangeSideAndLock - timeGetMaxGainNodeFromBucketlist) / (double)CLOCKS_PER_SEC << std::endl;
        // std::cout << "CopyMaxPartition time: " << (timeCopyMaxPartition - timeChangeSideAndLock) / (double)CLOCKS_PER_SEC << std::endl;
        // std::cout << "GetAdjNodes time: " << (timeGetAdjNodes - timeCopyMaxPartition) / (double)CLOCKS_PER_SEC << std::endl;
        // std::cout << "CalculateGain time: " << (timeCalculateGain - timeGetAdjNodes) / (double)CLOCKS_PER_SEC << std::endl;
        // std::cout << "Total time: " << (timeCalculateGain - timestart) / (double)CLOCKS_PER_SEC << std::endl;
        // GetMaxGainNodeFromBucketlist_timesum += timeGetMaxGainNodeFromBucketlist - timestart;
        // ChangeSideAndLock_timesum += timeChangeSideAndLock - timeGetMaxGainNodeFromBucketlist;
        // CopyMaxPartition_timesum += timeCopyMaxPartition - timeChangeSideAndLock;
        // GetAdjNodes_timesum += timeGetAdjNodes - timeCopyMaxPartition;
        // CalculateGain_timesum += timeCalculateGain - timeGetAdjNodes;
        // Total_timesum += timeCalculateGain - timestart;
        // std::cout << "--------------------------------\n\n";

        count++;
    }

    // std::cout << "GetMaxGainNodeFromBucketlist time: " << GetMaxGainNodeFromBucketlist_timesum / (double)CLOCKS_PER_SEC << std::endl;
    // std::cout << "ChangeSideAndLock time: " << ChangeSideAndLock_timesum / (double)CLOCKS_PER_SEC << std::endl;
    // std::cout << "CopyMaxPartition time: " << CopyMaxPartition_timesum / (double)CLOCKS_PER_SEC << std::endl;
    // std::cout << "GetAdjNodes time: " << GetAdjNodes_timesum / (double)CLOCKS_PER_SEC << std::endl;
    // std::cout << "CalculateGain time: " << CalculateGain_timesum / (double)CLOCKS_PER_SEC << std::endl;
    // std::cout << "Total time: " << Total_timesum / (double)CLOCKS_PER_SEC << std::endl;

    std::cout << "maxGainSum: " << maxGainSum << std::endl;
}

void Partition::iterate(int count)
{
    for (int i = 0; i < count; i++)
    {
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