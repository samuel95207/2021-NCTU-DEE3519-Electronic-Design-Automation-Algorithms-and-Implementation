#include <vector>
#include <climits>
#include <utility> 

#include "hypergraph.h"

class Partition
{
    class BucketList
    {
        int pinMax = 0;
        std::vector<std::vector<int>> bucket;
        std::unordered_map<int, std::pair<int, int>> bucketPosList;
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
    std::vector<bool> locklist;


    BucketList leftBucket;
    BucketList rightBucket;

public:
    Partition(HyperGraph *graph);

    void printPartition();
    void printBucket();

    int gain(int nodeId);
    void calculateGain();
    void changeSide(int nodeId);
    void lock(int nodeId);
};