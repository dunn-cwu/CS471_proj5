#ifndef __EXPERIMENT_H
#define __EXPERIMENT_H

#include <string>
#include "inireader.h"
#include "flowshopbasic.h"

namespace cs471
{
    struct TestParams
    {
        int minTestFile;
        int maxTestFile;
        int numThreads;
        int algorithm;
        std::string inputFilesDir;
        std::string resultsFile;
        std::string timesFile;
    };

    class Experiment
    {
    public:
        Experiment(std::string paramsFile);
        ~Experiment();

        int run();
    private:
        util::IniReader iniParams;

        int runNEHThreaded(const TestParams& p, const std::string inputFile, int testIndex);
        fshop::FlowshopBasic* allocFlowShop(const char* inputFile, int alg);
        TestParams readTestParams();
    };
}

#endif