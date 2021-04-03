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

    // Create Graph
    HyperGraph G;


    // Input
    ifstream infile(argv[1]);

    int netCount, nodesCount;
    infile >> netCount >> nodesCount;

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
    infile.close();

    // Input timer
    auto timeInput = std::clock();
    cout << "Input time: " << (timeInput - timestart) / (double)CLOCKS_PER_SEC << endl;


    // Create partition
    Partition P(&G, 0.46);

    // Set FM TLE
    int FM_TLE = 85 - (timeInput - timestart) / (double)CLOCKS_PER_SEC;

    // Iterate FM Algorithm
    int iterateCount = P.iterate(FM_TLE, INT_MAX, 7);

    // FM timer
    auto timeFM = std::clock();
    cout << "FM time: " << (timeFM - timeInput) / (double)CLOCKS_PER_SEC << endl;
    cout << "Iterate Count: " << iterateCount << endl;

    // Output
    ofstream outfile("output.txt");
    outfile << P;

    // Output timer
    auto timeOutput = std::clock();
    cout << "Output time: " << (timeOutput - timeFM) / (double)CLOCKS_PER_SEC << endl;
    cout << "Total time: " << (timeOutput - timestart) / (double)CLOCKS_PER_SEC << endl;
}