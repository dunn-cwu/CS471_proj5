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
#include "experiment.h"

using namespace std;

int main(int argc, char** argv)
{
    // Make sure we have enough command line args
    if (argc <= 1)
    {
        cout << "Error: Missing command line parameter." << endl;
        cout << "Proper usage: " << argv[0] << " [param file]" << endl;
        return EXIT_FAILURE;
    }

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
    
    return 0;
}

// =========================
// End of main.cpp
// =========================
