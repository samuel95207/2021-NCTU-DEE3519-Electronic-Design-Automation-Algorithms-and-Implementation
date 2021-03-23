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
        void insert(int nodeId, int gain);
        void update(int nodeId, int gain);
        void remove(int nodeId);

        void print();

        friend class Partition;
    };

    HyperGraph *graph;

    std::vector<bool> partition;
    int leftCount = 0;
    int rightCount = 0;

    std::vector<bool> locklist;
    int lockCount = 0;

    std::pair<int, int> areaConstrain;

    BucketList leftBucket;
    BucketList rightBucket;

    int gainSum = 0;
    int maxGainSum = INT_MIN;
    std::vector<bool> maxPartition;

public:
    Partition(HyperGraph *graph, double balanceFactor = 0.5);

    void printPartition();
    void printBucket();

    int gain(int nodeId);
    void calculateInitialGain();
    void calculateGain(std::set<int> nodes);
    void changeSide(int nodeId);
    void lock(int nodeId);
    bool checkBalance();
    bool checkLock(int nodeId);
    bool checkAllLock();

    void FM_Algorithm();
    std::pair<int, int> getMaxGainNodeFromBucketlist();

    friend std::ostream &operator<<(std::ostream &out, const Partition &P);
};