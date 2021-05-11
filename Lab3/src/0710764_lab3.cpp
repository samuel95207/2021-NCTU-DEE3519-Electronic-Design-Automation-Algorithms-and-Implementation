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
    BDDInterfacec()
    {
        gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
        Cudd_AutodynEnable(gbm, CUDD_REORDER_GENETIC);
        Cudd_ReduceHeap(gbm, CUDD_REORDER_GENETIC, 3000);
    }
    ~BDDInterfacec()
    {
        Cudd_Quit(gbm);
    }

    void setBooleanFunction(string &str)
    {
        int count = 0;

        DdNode *and_tmp = Cudd_ReadOne(gbm);
        Cudd_Ref(and_tmp);
        DdNode *or_tmp = Cudd_ReadLogicZero(gbm);
        Cudd_Ref(or_tmp);

        for (auto c : str)
        {
            if (isalpha(c))
            {
                if (nodeMap.find(tolower(c)) == nodeMap.end())
                {
                    DdNode *var = Cudd_bddNewVar(gbm);
                    DdNode *neg_var = Cudd_Not(var);
                    nodeMap[tolower(c)] = var;
                    nodeMap[toupper(c)] = neg_var;

                    inputOrderMap[count] = tolower(c);
                    count++;
                }

                DdNode *var = nodeMap[c];

                DdNode *tmp = Cudd_bddAnd(gbm, var, and_tmp);
                Cudd_Ref(tmp);
                Cudd_RecursiveDeref(gbm, and_tmp);
                and_tmp = tmp;
            }
            else if (c == '+')
            {
                DdNode *tmp = Cudd_bddOr(gbm, and_tmp, or_tmp);
                Cudd_Ref(tmp);
                Cudd_RecursiveDeref(gbm, or_tmp);
                or_tmp = tmp;

                and_tmp = Cudd_ReadOne(gbm);
                Cudd_Ref(and_tmp);
            }
        }
        DdNode *tmp = Cudd_bddOr(gbm, and_tmp, or_tmp);
        Cudd_Ref(tmp);
        Cudd_RecursiveDeref(gbm, or_tmp);
        Cudd_RecursiveDeref(gbm, and_tmp);

        bdd = tmp;
    }

    void setProbability(char c, double probability)
    {
        probabilityMap[c] = probability;
    }

    double calculateProbability(string tmpFilename)
    {
        auto saveSTDOUT = dup(STDOUT_FILENO);
        auto fp = freopen(tmpFilename.c_str(), "w", stdout);

        Cudd_PrintMinterm(gbm, bdd);

        fclose(fp);
        fclose(stdout);
        stdout = fdopen(saveSTDOUT, "w");

        ifstream mintermFile(tmpFilename);

        probability = 0;

        string line;
        while (getline(mintermFile, line))
        {
            istringstream iss;
            string minterm;

            iss.str(line);
            iss >> minterm;

            cerr<<minterm<<endl;

            double probability_tmp = 1;
            int size = minterm.size();
            for (int i = 0; i < size; i++)
            {
                char c = minterm[i];
                if (c == '1')
                {
                    probability_tmp *= probabilityMap[inputOrderMap[i]];  
                }
                else if (c == '0')
                {
                    probability_tmp *= 1 - probabilityMap[inputOrderMap[i]];   
                }
            }
            probability += probability_tmp;
        }

        cerr<<probability<<endl;

        mintermFile.close();

        // clear trace in output file
        ofstream clearFile(tmpFilename);
        clearFile<<"";
        clearFile.close();

        return probability;
    }

    void printBDD(int n, int pr = 4)
    {
        DdNode *add = Cudd_BddToAdd(gbm, bdd);

        printf("DdManager nodes: %ld | ", Cudd_ReadNodeCount(gbm));        /*Reports the number of live nodes in BDDs and ADDs*/
        printf("DdManager vars: %d | ", Cudd_ReadSize(gbm));               /*Returns the number of BDD variables in existence*/
        printf("DdManager reorderings: %d | ", Cudd_ReadReorderings(gbm)); /*Returns the number of times reordering has occurred*/
        printf("DdManager memory: %ld \n", Cudd_ReadMemoryInUse(gbm));     /*Returns the memory in use by the manager measured in bytes*/
        Cudd_PrintDebug(gbm, add, n, pr);                                  // Prints to the standard output a DD and its statistics: number of nodes, number of leaves, number of minterms.
    }

    void printProbabilityMap()
    {
        cout << "{\n";
        for (auto item : probabilityMap)
        {
            cout << "  " << item.first << ": " << item.second << endl;
        }
        cout << "}\n";
    }

    void printVariables()
    {
        cout << "{\n";
        for (auto item : inputOrderMap)
        {
            cout << "  " << item.first << ": " << item.second << endl;
        }
        cout << "}\n";
        Cudd_PrintMinterm(gbm, bdd);
    }

    void outputGraphFile(char *filename)
    {
        DdNode *add = Cudd_BddToAdd(gbm, bdd);
        FILE *outfile; // output file pointer for .dot file
        outfile = fopen(filename, "w");
        DdNode **ddnodearray = (DdNode **)malloc(sizeof(DdNode *)); // initialize the function array
        ddnodearray[0] = add;
        Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile); // dump the function to .dot file

        free(ddnodearray);
        fclose(outfile); // close the file */
    }

    friend std::istream &operator>>(std::istream &in, BDDInterfacec &BDD)
    {
        string line;

        getline(in, line);

        BDD.setBooleanFunction(line);

        while (getline(in, line))
        {
            char c;
            double probability;
            istringstream iss;
            iss.str(line);
            iss >> c >> probability;
            BDD.setProbability(tolower(c), probability);
        }

        return in;
    }

    friend std::ostream &operator<<(std::ostream &out, const BDDInterfacec &BDD)
    {
        out<<BDD.probability;
        return out;
    }
};

int main(int argc, char *argv[])
{
    BDDInterfacec BDD;
    ifstream infile(argv[1]);
    infile >> BDD;
    infile.close();

    // BDD.printProbabilityMap();
    // BDD.printBDD(2, 4);
    // BDD.printVariables();
    // BDD.outputGraphFile("graph.dot");

    BDD.calculateProbability(argv[2]);


    ofstream outfile(argv[2]);
    outfile<<BDD;

}