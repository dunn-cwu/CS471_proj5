/**
 * @file experiment.cpp
 * @author Andrew Dunn (Andrew.Dunn@cwu.edu)
 * @brief Implementation file for the Experiment class.
 * @version 0.1
 * @date 2019-05-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <stdexcept>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
#include "experiment.h"
#include "threadpool.h"
#include "stringutils.h"
#include "flowshopblocking.h"
#include "flowshopnowait.h"
#include "neh.h"

#define INI_TEST_SECTION      "test"
#define INI_TEST_MINFILE      "minTestFile"
#define INI_TEST_MAXFILE      "maxTestFile"
#define INI_TEST_NUMTHREADS   "numThreads"
#define INI_TEST_ALGORITHM    "algorithm"
#define INI_TEST_INPUTFILEDIR "inputFilesDir"
#define INI_TEST_RESULTSFILE  "resultsFile"
#define INI_TEST_TIMESFILE    "timesFile"

using namespace cs471;
using namespace fshop;
using namespace util;
using namespace std;
using namespace chrono;

/**
 * @brief Construct a Experiment object
 * 
 * @param paramsFile File path to the input ini paramater file
 */
Experiment::Experiment(string paramsFile)
{
    // Attempt to open parameters file
    if (!iniParams.openFile(paramsFile))
    {
        string msg = "Error opening ini file: ";
        msg += paramsFile;
        throw std::runtime_error(msg);
    }

    cout << "Loaded parameters file: " << paramsFile << endl;
}

/**
 * @brief Runs the cs471 lab 5 experiment, which involves executing
 * the NEH algorithm for a specific flowshop objective function that
 * is specified in the input parameters file.
 * 
 * @return int Returns a non-zero error code on failure. Otherwise returns zero.
 */
int Experiment::runNEH()
{
    // Retrieve test parameters from ini file
    TestParams p = readTestParams();

    // Construct data table to store experiment results
    mdata::DataTable<string> resultsTable(p.maxTestFile - p.minTestFile + 1, 6);

    // Initialize thread pool with a parameter-given number of threads
    ThreadPool tpool(p.numThreads);

    // Initialize thread future vector, used for thread pool synchronization
    // and keeps track of the individual tasks being executed.
    vector<std::future<int>> futures;

    cout << "Started " << p.numThreads << " worker threads ..." << endl;

    if (p.algorithm == 1)
        cout << "Running NEH on Flow Shop with Blocking ..." << endl;
    else if (p.algorithm == 2)
        cout << "Running NEH on Flow Shop with No Wait ..." << endl;
    else
        cout << "Running NEH on Flow Shop Scheduling ..." << endl;

    // Prepare results table column header labels
    resultsTable.setColLabel(0, "Data Set");
    resultsTable.setColLabel(1, "cMax");
    resultsTable.setColLabel(2, "TFT");
    resultsTable.setColLabel(3, "Func Calls");
    resultsTable.setColLabel(4, "Execution Time (ms)");
    resultsTable.setColLabel(5, "Sequence");

    // Add all input test files as tasks in thread pool
    for (int i = p.minTestFile; i <= p.maxTestFile; i++)
    {
        string inputFile = std::to_string(i) + ".txt";
        futures.emplace_back(
            tpool.enqueue(&cs471::Experiment::runNEHThreaded, this, &p, inputFile, i, &resultsTable)
        );
    }

    // const size_t totalFutures = futures.size();

    // Join all thread pool tasks using futures vector
    // and get the return value for each
    for (int i = 0; i < futures.size(); i++)
    {
        int err = futures[i].get();
        if (err)
        {
            // Threaded task returned with an error code, bail
            tpool.stopAndJoinAll();
            return err;
        }
    }

    // Output results table to a csv file
    if (!p.resultsFile.empty())
    {
        resultsTable.exportCSV(p.resultsFile.c_str());
        cout << "Results exported to: " << p.resultsFile << endl;
    }

    return 0;
}

/**
 * @brief Runs a single instance of the NEH algorithm.
 * This function should only be executed from within an async thread.
 * 
 * @param p Pointer to the experiment test parameters
 * @param inputFile Input file containing the job processing time matrix
 * @param testIndex Index of the input test file, used to store results in results table on correct row
 * @param resultsTable Pointer to the results table which this function will place it's NEH results into
 * @return int 
 */
int Experiment::runNEHThreaded(TestParams* const p, const std::string inputFile, int testIndex, mdata::DataTable<std::string>* resultsTable)
{
    string fullInputPath = p->inputFilesDir + inputFile;

    // Get the flowshop objective function that we want to optimize
    auto objectiveFs = allocFlowShop(fullInputPath.c_str(), p->algorithm);
    if (objectiveFs == nullptr)
        return 1;

    // Prepare pointer to results
    fsSol result = nullptr;

    // Start recording execution time
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    try
    {
        // Run the NEH algorithm on the objective flowshop function
        NEH neh;
        result = neh.run(objectiveFs);
    }
    catch(const std::exception& e)
    {
        std::cerr << "An exception occurred while running NEH:" << endl;
        std::cerr << e.what() << endl;
        std::cerr << "Input file: " << inputFile << endl;
        return 2;
    }
    
    // Record execution time
    high_resolution_clock::time_point t_end = high_resolution_clock::now();
    double execTimeMs = static_cast<double>(duration_cast<nanoseconds>(t_end - t_start).count()) / 1000000.0;

    // Insert NEH results into results table at the correct row
    resultsTable->setEntry(testIndex, 0, std::to_string(testIndex));
    resultsTable->setEntry(testIndex, 1, std::to_string(result->cmax));
    resultsTable->setEntry(testIndex, 2, std::to_string(result->totalFlowTime));
    resultsTable->setEntry(testIndex, 3, std::to_string(objectiveFs->getFuncCallCounts()));
    resultsTable->setEntry(testIndex, 4, std::to_string(execTimeMs));
    resultsTable->setEntry(testIndex, 5, result->getJobSeqAsString());


    // ======= GANTT STUFF =======
    /*
    const size_t numMachines = objectiveFs->getTotalMachines();
    const size_t numJobs = objectiveFs->getTotalJobs();
    
    auto startTimeMatrix = result->getStartTimeMatrix();
    auto departTimeMatrix = result->getDepartTimeMatrix();

    mdata::DataTable<std::string> ganttTable(numJobs * numMachines, 5);

    ganttTable.setColLabel(0, "Item");
    ganttTable.setColLabel(1, "Machine");
    ganttTable.setColLabel(2, "Job");
    ganttTable.setColLabel(3, "Start");
    ganttTable.setColLabel(4, "End");

    size_t row = 0;

    for (size_t m = 0; m < numMachines; m++)
    {
        for (size_t j = 0; j < numJobs; j++)
        {
            ganttTable.setEntry(row, 0, std::to_string(row + 1));
            ganttTable.setEntry(row, 1, std::string("Machine ") + std::to_string(m + 1));
            ganttTable.setEntry(row, 2, std::string("Job ") + std::to_string(j + 1));
            ganttTable.setEntry(row, 3, std::to_string(startTimeMatrix[m][j]));
            ganttTable.setEntry(row, 4, std::to_string(departTimeMatrix[m][j]));

            row++;
        }
    }

    std::string ganttfile = "results/gantt/";

    if (p->algorithm == 0)
        ganttfile += "fss/";
    else if (p->algorithm == 1)
        ganttfile += "fsb/";
    else
        ganttfile += "fsnw/";

    ganttfile += std::to_string(testIndex);
    ganttfile += "-gantt.csv";
    ganttTable.exportCSV(ganttfile.c_str());
    */
    // ===========================


    // Dump NEH results start and departure time matrices to a csv file
    if (!p->timesFile.empty())
        result->outputTimesCsv(util::s_replace(p->timesFile, "%TEST%", std::to_string(testIndex)));

    // Clean up allocated memory
    delete objectiveFs;

    return 0;
}

/**
 * @brief Allocates a new flowshop object depending on the selected algorithm and
 * returns a pointer to the newly created object.
 * 
 * @param inputFile Input file that contains the job processing time matrix which will be passed to the flowshop object
 * @param alg Index of the flowshop algorithm to allocate. 0 = Standard, 1 = With Blocking, 2 = With No Wait.
 * @return Returns a pointer to the newly created flowshop object
 */
FlowshopBasic* Experiment::allocFlowShop(const char* inputFile, int alg)
{
    FlowshopBasic* objectiveFs = nullptr;

    switch (alg)
    {
        case 0:
            objectiveFs = new FlowshopBasic(inputFile);
            break;
        case 1:
            objectiveFs = new FlowshopBlocking(inputFile);
            break;
        case 2:
            objectiveFs = new FlowshopNoWait(inputFile);
            break;
    }

    return objectiveFs;
}

/**
 * @brief Reads the experiment test parameters in from the ini param file
 * and places them in a TestParams struct.
 * 
 * @return Returns the experiment test parameters in a TestParams struct
 */
TestParams Experiment::readTestParams()
{
    TestParams p = { };

    p.minTestFile = iniParams.getEntryAs<int>(INI_TEST_SECTION, INI_TEST_MINFILE, 0);
    p.maxTestFile = iniParams.getEntryAs<int>(INI_TEST_SECTION, INI_TEST_MAXFILE, 120);
    p.numThreads = iniParams.getEntryAs<int>(INI_TEST_SECTION, INI_TEST_NUMTHREADS, 1);
    p.algorithm = iniParams.getEntryAs<int>(INI_TEST_SECTION, INI_TEST_ALGORITHM, 0);
    p.inputFilesDir = iniParams.getEntry(INI_TEST_SECTION, INI_TEST_INPUTFILEDIR, "");
    p.resultsFile = iniParams.getEntry(INI_TEST_SECTION, INI_TEST_RESULTSFILE, "");
    p.timesFile = iniParams.getEntry(INI_TEST_SECTION, INI_TEST_TIMESFILE, "");

    // Check bounds for numThreads
    if (p.numThreads < 1 || p.numThreads > 16)
    {
        cout << "Warning: Number of threads invalid. Defaulting to default 1 threads." << endl;
        p.numThreads = 1;
    }

    // Check bounds for algorithm selection
    if (p.algorithm < 0 || p.algorithm > 2)
    {
        cout << "Warning: Algorithm selection invalid. Defaulting to algorithm 0." << endl;
        p.algorithm = 0;
    }

    return p;
}

/**
 * @brief Used for debugging the objective flowshop functions.
 * This method runs the objective function specified in the
 * parameters file with the specified input processing time files
 * and then prints the results of the flowshop objective function
 * with the given job sequence.
 * 
 * @param seq Job sequence to run flowshop objective functions with
 * @param seqSize Size of the job sequence array
 * @return Returns a non-zero error code on failure, otherwise zero.
 */
int Experiment::runDebugSeq(int* seq, size_t seqSize)
{
    // Retrieve test parameters from ini file
    TestParams p = readTestParams();

    if (p.algorithm == 1)
        cout << "Running Flow Shop with Blocking ..." << endl;
    else if (p.algorithm == 2)
        cout << "Running Flow Shop with No Wait ..." << endl;
    else
        cout << "Running Flow Shop Scheduling ..." << endl;

    cout << endl;

    // Prepare pointer to results
    fsSol result = nullptr;

    for (int i = p.minTestFile; i <= p.maxTestFile; i++)
    {
        string fullInputPath = p.inputFilesDir + std::to_string(i) + ".txt";

        cout << "Input file: " << fullInputPath << endl;

        // Get the flowshop objective function that we want to optimize
        auto objectiveFs = allocFlowShop(fullInputPath.c_str(), p.algorithm);
        if (objectiveFs == nullptr)
        {
            cout << "Objective flowshop function encountered an error." << endl;
            return 1;
        }

        result = objectiveFs->calcObjective(seq, seqSize);
        result->outputAll(std::cout);

        delete objectiveFs;

        cout << "=======================================" << endl;
    }

    cout << "Debug objective function sequence tests completed." << endl;

    return 0;
}

// =========================
// End of experiment.cpp
// =========================
