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

    infile.close();

    auto timeInput = std::clock();
    cout << "Input time: " << (timeInput - timestart) / (double)CLOCKS_PER_SEC << endl;

    Partition P(&G, 0.46);
    int iterateCount = P.iterate(85 - (timeInput - timestart) / (double)CLOCKS_PER_SEC);

    auto timeGain = std::clock();
    cout << "FM time: " << (timeGain - timeInput) / (double)CLOCKS_PER_SEC << endl;
    cout << "Iterate Count: " << iterateCount << endl;


    ofstream outfile("output.txt");
    outfile << P;

    auto timeOutput = std::clock();
    cout << "Output time: " << (timeOutput - timeGain) / (double)CLOCKS_PER_SEC << endl;
    cout << "Total time: " << (timeOutput - timestart) / (double)CLOCKS_PER_SEC << endl;
}