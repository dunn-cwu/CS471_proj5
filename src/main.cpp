/**
 * @file main.cpp
 * @author Andrew Dunn (Andrew.Dunn@cwu.edu)
 * @brief Program entry point, runs the cs471 project 5
 * experiment via experiment.h.
 * @version 0.1
 * @date 2019-05-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <iostream>
#include <sstream>
#include <vector>
#include <set>

#include "experiment.h"

using namespace std;

int runDebugJobSeq(const char* seq);

int main(int argc, char** argv)
{
    // Make sure we have enough command line args
    if (argc <= 1)
    {
        cout << "Error: Missing command line parameter." << endl;
        cout << "Proper usage: " << argv[0] << " [param file] \"[Debug Job Sequence]\"" << endl;
        cout << "The debug job sequence is optional, and must be passed in the form \"1 2 3 4 5\" as a single argument, where the values are the jobs separated by spaces." << endl;
        return EXIT_FAILURE;
    }

    if (argc > 2)
    {
        return runDebugJobSeq(argv[2]);
    }
    else
    {
        try
        {
            // Run experiment and return error code
            cs471::Experiment ex(argv[1]);
            return ex.run();
        }
        catch(const std::exception& e)
        {
            std::cerr << "An exception occured:" << endl;
            std::cerr << e.what() << endl;
        }
    }
    
    return 0;
}

int runDebugJobSeq(const char* seq)
{
    string strSeq = seq;
    vector<int> jobSeq;

    stringstream ss(strSeq);
    int val;

    while (ss >> val)
    {
        jobSeq.push_back(val);
    }

    if (jobSeq.size() == 0)
    {
        cerr << "Error: debug job sequence is missing or invalid." << endl;
        return 1;
    }

    set<int> permCheckSet(jobSeq.begin(), jobSeq.end());
    if (permCheckSet.size() != jobSeq.size())
    {
        cerr << "Error: debug job sequence has duplicate jobs in permutation." << endl;
        return 2;
    }

    return 0;
}

// =========================
// End of main.cpp
// =========================
