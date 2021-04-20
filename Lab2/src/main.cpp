#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <ctime>

#include "Grid.h"

using namespace std;

int main(int argc, char **argv)
{

    Grid G;

    // Input
    ifstream infile(argv[1]);

    infile>>G;
    cout<<G;

}