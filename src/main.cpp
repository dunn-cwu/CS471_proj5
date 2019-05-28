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