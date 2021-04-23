#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <ctime>

#include "Router.h"

using namespace std;

int main(int argc, char **argv)
{
    Router R;

    // Input
    ifstream infile(argv[1]);
    infile >> R;
    infile.close();

    R.start();

    // Output
    ofstream outfile(argv[2]);
    outfile << R;
    cout << "\nOutput:\n"
         << R;
    outfile.close();
}