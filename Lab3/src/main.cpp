#include <iostream>

#include "util.h"
#include "cudd.h"

#include "BDDInterface.h"

using namespace std;

int main(int argc, char *argv[])
{
    BDDInterfacec BDD;
    ifstream infile(argv[1]);
    infile >> BDD;
    infile.close();

    // BDD.printProbabilityMap();
    // BDD.printBDD(2, 4);
    // BDD.printVariables();
    // BDD.writeGraphFile("graph.dot");

    BDD.calculateProbability(argv[2]);

    cerr<<BDD<<endl;

    ofstream outfile(argv[2]);
    outfile<<BDD;

}