/**
 * @file experiment.h
 * @author Andrew Dunn (Andrew.Dunn@cwu.edu)
 * @brief Contains the Experiment class which runs the cs471 lab 5 experiment
 * @version 0.1
 * @date 2019-05-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef __EXPERIMENT_H
#define __EXPERIMENT_H

#include <string>
#include "inireader.h"
#include "datatable.h"
#include "flowshopbasic.h"

namespace cs471
{
    /**
     * @brief Simple data structure that stores the 
     * test parameters for the experiment
     */
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

    /**
     * @brief The experiment class runs takes a given ini file path,
     * opens it, parses the parameters, then runs the NEH algorithm
     * with the given parameters.
     */
    class Experiment
    {
    public:
        Experiment(std::string paramsFile);
        ~Experiment() = default;

        int run();
    private:
        util::IniReader iniParams;

        int runNEHThreaded(TestParams* const p, const std::string inputFile, int testIndex, mdata::DataTable<std::string>* resultsTable);
        fshop::FlowshopBasic* allocFlowShop(const char* inputFile, int alg);
        TestParams readTestParams();
    };
}

#endif

// =========================
// End of experiment.h
// =========================
