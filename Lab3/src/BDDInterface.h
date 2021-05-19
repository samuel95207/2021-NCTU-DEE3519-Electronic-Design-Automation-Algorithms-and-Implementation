#include <iostream>
#include <map>
#include <unordered_map>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>

#include "util.h"
#include "cudd.h"

using namespace std;

class BDDInterfacec
{
    DdManager *gbm;
    DdNode *bdd;
    unordered_map<char, DdNode *> nodeMap;
    unordered_map<char, double> probabilityMap;
    unordered_map<int, char> inputOrderMap;

    double probability;

public:
    BDDInterfacec();
    ~BDDInterfacec();

    void setBooleanFunction(string &str);
    void setProbability(char c, double probability);

    double calculateProbability();
    double calculateProbability_minterm(string tmpFilename);

    void printBDD(int n, int pr);
    void printProbabilityMap();
    void printVariables();

    void writeGraphFile(char *filename);

    friend std::istream &operator>>(std::istream &in, BDDInterfacec &BDD);
    friend std::ostream &operator<<(std::ostream &out, const BDDInterfacec &BDD);
};