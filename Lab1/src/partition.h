#include <vector>
#include <climits>

#include "hypergraph.h"

class Partition
{
    class BucketList
    {
        int pinMax = 0;
        std::vector<std::vector<int>> bucket;
        int maxGain = INT_MIN;

    public:
        BucketList();

        void setPmax(int pinMax);

        void clear();
        void insert(int gain, int nodeId);
        void print();
    };

    HyperGraph *graph;
    std::vector<bool> partition;

    BucketList leftBucket;
    BucketList rightBucket;

public:
    Partition(HyperGraph *graph);

    void printPartition();
    void printBucket();

    int gain(int nodeId);
    void calculateGain();
};