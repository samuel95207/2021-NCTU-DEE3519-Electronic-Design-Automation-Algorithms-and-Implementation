#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <ctime>

#include "./partition.h"


using namespace std;

int main(int argc, char **argv)
{

    // Timer
    auto timestart = clock();

    ifstream infile(argv[1]);

    int netCount, nodesCount;
    infile >> netCount >> nodesCount;

    HyperGraph G;

    string input_str;
    while (getline(infile, input_str))
    {
        istringstream iss;
        iss.str(input_str);

        auto nodes = new set<int>;

        int node;
        while (iss >> node)
        {
            nodes->insert(node);
        }

        G.addEdge(nodes, 1);
    }

    auto timeIO = std::clock();
    cout << "IO time: " << (timeIO - timestart) / (double)CLOCKS_PER_SEC << endl;


    Partition P(&G);
    P.calculateGain();

    auto timeGain = std::clock();
    cout << "gain time: " << (timeGain - timeIO) / (double)CLOCKS_PER_SEC << endl;

    P.printPartition();
    P.printBucket();

    // cout<<G<<endl;



    // cout<<G<<endl;
    // cout<<G.getEdge(1,3)<<endl;
}