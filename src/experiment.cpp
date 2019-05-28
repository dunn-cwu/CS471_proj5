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

Experiment::Experiment(string paramsFile)
{
    if (!iniParams.openFile(paramsFile))
    {
        string msg = "Error opening ini file: ";
        msg += paramsFile;
        throw std::runtime_error(msg);
    }

    cout << "Loaded parameters file: " << paramsFile << endl;
}

Experiment::~Experiment()
{
}

int Experiment::run()
{
    TestParams p = readTestParams();
    mdata::DataTable<string> resultsTable(p.maxTestFile - p.minTestFile + 1, 5);
    ThreadPool tpool(p.numThreads);
    vector<std::future<int>> futures;

    cout << "Started " << p.numThreads << " worker threads ..." << endl;

    if (p.algorithm == 1)
        cout << "Running NEH on Flow Shop with Blocking ..." << endl;
    else if (p.algorithm == 2)
        cout << "Running NEH on Flow Shop with No Wait ..." << endl;
    else
        cout << "Running NEH on Flow Shop Scheduling ..." << endl;

    resultsTable.setColLabel(0, "Data Set");
    resultsTable.setColLabel(1, "cMax");
    resultsTable.setColLabel(2, "TFT");
    resultsTable.setColLabel(3, "Execution Time (ms)");
    resultsTable.setColLabel(4, "Sequence");

    for (int i = p.minTestFile; i <= p.maxTestFile; i++)
    {
        string inputFile = std::to_string(i) + ".txt";
        futures.emplace_back(
            tpool.enqueue(&cs471::Experiment::runNEHThreaded, this, &p, inputFile, i, &resultsTable)
        );
    }

    const size_t totalFutures = futures.size();

    for (int i = 0; i < futures.size(); i++)
    {
        int err = futures[i].get();
        if (err)
        {
            tpool.stopAndJoinAll();
            return err;
        }
    }

    if (!p.resultsFile.empty())
    {
        resultsTable.exportCSV(p.resultsFile.c_str());
        cout << "Results exported to: " << p.resultsFile << endl;
    }

    return 0;
}

int Experiment::runNEHThreaded(TestParams* const p, const std::string inputFile, int testIndex, mdata::DataTable<std::string>* resultsTable)
{
    string fullInputPath = p->inputFilesDir + inputFile;
    auto objectiveFs = allocFlowShop(fullInputPath.c_str(), p->algorithm);
    if (objectiveFs == nullptr)
        return 1;

    fsSol result = nullptr;

    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    try
    {
        result = NEH::run(objectiveFs);
    }
    catch(const std::exception& e)
    {
        std::cerr << "An exception occurred while running NEH:" << endl;
        std::cerr << e.what() << endl;
        std::cerr << "Input file: " << inputFile << endl;
        return 2;
    }
    
    high_resolution_clock::time_point t_end = high_resolution_clock::now();
    double execTimeMs = static_cast<double>(duration_cast<nanoseconds>(t_end - t_start).count()) / 1000000.0;

    resultsTable->setEntry(testIndex, 0, std::to_string(testIndex));
    resultsTable->setEntry(testIndex, 1, std::to_string(result->cmax));
    resultsTable->setEntry(testIndex, 2, std::to_string(result->totalFlowTime));
    resultsTable->setEntry(testIndex, 3, std::to_string(execTimeMs));
    resultsTable->setEntry(testIndex, 4, result->getJobSeqAsString());

    if (!p->timesFile.empty())
        result->outputTimesCsv(util::s_replace(p->timesFile, "%TEST%", std::to_string(testIndex)));

    delete objectiveFs;

    return 0;
}

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

    if (p.numThreads < 1 || p.numThreads > 16)
    {
        cout << "Warning: Number of threads invalid. Defaulting to default 1 threads." << endl;
        p.numThreads = 1;
    }

    if (p.algorithm < 0 || p.algorithm > 2)
    {
        cout << "Warning: Algorithm selection invalid. Defaulting to algorithm 0." << endl;
        p.algorithm = 0;
    }

    return p;
}