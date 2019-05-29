/**
 * @file flowshopbasic.cpp
 * @author Andrew Dunn (Andrew.Dunn@cwu.edu)
 * @brief Implementation file for the FlowshopBasic class.
 * @version 0.1
 * @date 2019-05-24
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <stdexcept>
#include <fstream>
#include "flowshopbasic.h"
#include "mem.h"

using namespace fshop;

/**
 * @brief Simple inline helper function that returns the max of two integers
 * 
 * @param val1 First integer
 * @param val2 Second integer
 * @return Returns the maximum of the two integers
 */
inline int max(int val1, int val2)
{
    if (val1 >= val2) return val1;
    else return val2;
}

// ============================================================

/**
 * @brief Constructs a new SlowshopSolution object
 * 
 * @param _startimeMatrix Pointer to the start times matrix. This class takes ownership of the pointer and will destroy it.
 * @param _departTimeMatrix Pointer to the departure times matrix. This class takes ownership of the pointer and will destroy it.
 * @param _tMatrixRows Number of rows (machines) in the start and departure time matrices
 * @param _jobSeq Pointer to the job sequence array. This class takes ownership of the pointer and will destroy it.
 * @param _seqSize Size of the job sequence array
 * @param _cmax Cmax value of the flowshop result
 * @param _totalFlowTime Total flow time of the flowshop result
 */
FlowshopSolution::FlowshopSolution(int** _startimeMatrix, int** _departTimeMatrix, size_t _tMatrixRows, int* _jobSeq, size_t _seqSize, int _cmax, int _totalFlowTime)
    : startTimeMatrix(_startimeMatrix), departTimeMatrix(_departTimeMatrix), numMachines(_tMatrixRows), seqSize(_seqSize), cmax(_cmax), totalFlowTime(_totalFlowTime)
{
    if (_jobSeq == nullptr)
        throw std::invalid_argument("Error: _jobSeq cannot be nullptr");
    else if (_startimeMatrix == nullptr)
        throw std::invalid_argument("Error: _startimeMatrix cannot be nullptr");
    else if (_departTimeMatrix == nullptr)
        throw std::invalid_argument("Error: _departTimeMatrix cannot be nullptr");
    else if (seqSize == 0)
        throw std::invalid_argument("Error: _seqSize cannot be zero");

    jobSequence = util::allocArray<int>(seqSize);
    for (size_t i = 0; i < seqSize; i++)
        jobSequence[i] = _jobSeq[i];
}

/**
 * @brief Destroys the FlowshopSolution object
 */
FlowshopSolution::~FlowshopSolution()
{
    util::releaseArray<int>(jobSequence);
    util::releaseMatrix<int>(startTimeMatrix, numMachines);
    util::releaseMatrix<int>(departTimeMatrix, numMachines);
}

/**
 * @brief Returns a const pointer to the job sequence array
 * 
 * @return Returns a const pointer to the job sequence array
 */
const int* const FlowshopSolution::getJobSeq()
{
    return const_cast<const int* const>(jobSequence);
}

/**
 * @brief Returns the job sequence array as a string
 * 
 * @return Returns the job sequence array as a string
 */
std::string FlowshopSolution::getJobSeqAsString()
{
    std::string retStr = "[";

    for (size_t i = 0; i < seqSize; i++)
    {
        retStr += std::to_string(jobSequence[i]);
        if (i < seqSize - 1) retStr += "-";
    }

    retStr += "]";
    return retStr;
}

/**
 * @brief Returns a const pointer to the start times matrix.
 * 
 * @return Returns a const pointer to the start times matrix.
 */
const int** const FlowshopSolution::getStartTimeMatrix()
{
    return const_cast<const int** const>(startTimeMatrix);
}

/**
 * @brief Returns a const pointer to the departure times matrix.
 * 
 * @return Returns a const pointer to the departure times matrix.
 */
const int** const FlowshopSolution::getDepartTimeMatrix()
{
    return const_cast<const int** const>(departTimeMatrix);
}

/**
 * @brief Dumps the start times and departure times matrices to a csv file
 * that starts with the given fileNamePrefix.
 * 
 * @param fileNamePrefix Start of the path/file which will contain the data
 * @return Returns true on success. Otherwise false.
 */
bool FlowshopSolution::outputTimesCsv(const std::string& fileNamePrefix)
{
    using namespace std;

    // Create file name strings
    string startTimesFile = fileNamePrefix + "starttimes.csv";
    string departTimesFile = fileNamePrefix + "departtimes.csv";

    // Open files
    ofstream startOs = ofstream(startTimesFile, ios::trunc | ios::out);
    if (!startOs.good()) return false;

    ofstream departOs = ofstream(departTimesFile, ios::trunc | ios::out);
    if (!departOs.good()) return false;

    // Output start times and departure times data to the files
    for (size_t m = 0; m < numMachines; m++)
    {
        for (size_t j = 0; j < seqSize; j++)
        {
            startOs << startTimeMatrix[m][j];
            departOs << departTimeMatrix[m][j];

            if (j < seqSize - 1)
            {
                startOs << ",";
                departOs << ",";
            }
        }

        startOs << endl;
        departOs << endl;
    }

    // Close file handles and return
    startOs.close();
    departOs.close();
    return true;
}

/**
 * @brief Outputs all results data to the given stream in a human readable format
 * 
 * @param os The output stream to write to
 */
void FlowshopSolution::outputAll(std::ostream& os)
{
    std::cout << "Input seq: ";

    for (size_t i = 0; i < seqSize; i++)
    {
        std::cout << jobSequence[i];
        if (i < seqSize - 1)
            std::cout << ", ";
    }

    std::cout << std::endl;

    std::cout << "Cmax: " << cmax << std::endl;
    std::cout << "TFT: " << totalFlowTime << std::endl << std::endl;

    std::cout << "Starting times matrix:" << std::endl;
    util::outputMatrix(std::cout, startTimeMatrix, numMachines, seqSize, 4);
    std::cout << std::endl;

    std::cout << "Departure times matrix:" << std::endl;
    util::outputMatrix(std::cout, departTimeMatrix, numMachines, seqSize, 4);
    std::cout << std::endl;
}

/**
 * @brief Copy constructor for the FlowshopSolution class
 */
FlowshopSolution::FlowshopSolution(const FlowshopSolution& obj)
    : startTimeMatrix(obj.startTimeMatrix), departTimeMatrix(obj.departTimeMatrix), numMachines(obj.numMachines), seqSize(obj.seqSize), cmax(obj.cmax), totalFlowTime(obj.totalFlowTime)
{
    if (obj.jobSequence == nullptr)
        throw std::invalid_argument("Error: jobSequence cannot be nullptr");
    else if (seqSize == 0)
        throw std::invalid_argument("Error: seqSize cannot be zero");

    jobSequence = util::allocArray<int>(seqSize);
    for (size_t i = 0; i < seqSize; i++)
        jobSequence[i] = obj.jobSequence[i];
}

/**
 * @brief Move constructor for the FlowshopSolution class
 */
FlowshopSolution::FlowshopSolution(FlowshopSolution&& obj)
    : numMachines(obj.numMachines), seqSize(obj.seqSize), cmax(obj.cmax), totalFlowTime(obj.totalFlowTime)
{
    jobSequence = obj.jobSequence;
    startTimeMatrix = obj.startTimeMatrix;
    departTimeMatrix = obj.departTimeMatrix;
    obj.jobSequence = nullptr;
    obj.startTimeMatrix = nullptr;
    obj.departTimeMatrix = nullptr;
}

// ============================================================

/**
 * @brief Constructs a new FlowshopBasic object
 * 
 * @param procTimeMatrixFile File path to the file containing the job processing times matrix
 */
FlowshopBasic::FlowshopBasic(const char* procTimeMatrixFile)
    : startTimeMatrix(nullptr), ptMatrixRows(0), ptMatrixCols(0), funcCallCounter(0)
{
    // Attempt to load job processing times from the given file
    procTimeMatrix = util::loadMatrixFromFile<int>(procTimeMatrixFile, ptMatrixRows, ptMatrixCols);
    if (procTimeMatrix == nullptr)
    {
        std::string msg = "Error when loading matrix file: ";
        msg += procTimeMatrixFile;
        throw std::runtime_error(msg);
    }
}

/**
 * @brief Destroys the FlowshopBasic object
 * 
 */
FlowshopBasic::~FlowshopBasic()
{
    util::releaseMatrix<int>(procTimeMatrix, ptMatrixRows);
}

/**
 * @brief Returns the processing time for the given job on the given machine
 * 
 * @param machine Number of machine [1-n]
 * @param job Number of job [1-n]
 * @return Returns the processing time
 */
int FlowshopBasic::getProcessingTime(size_t machine, size_t job)
{
    if (machine == 0 || job == 0)
    {
        std::string msg = "Error: Machine or job number cannot be zero";
        throw std::out_of_range(msg);
    }
    else if (machine > ptMatrixRows || job > ptMatrixCols)
    {
        std::string msg = "Error: Machine or job number out of range";
        throw std::out_of_range(msg);
    }

    return procTimeMatrix[machine - 1][job - 1];
}

/**
 * @brief Returns the total number of jobs in the jobs processing time matrix
 * 
 * @return Returns the total number of jobs
 */
size_t FlowshopBasic::getTotalJobs()
{
    return ptMatrixCols;
}

/**
 * @brief Returns the total number of machines in the jobs processing time matrix
 * 
 * @return Returns the total number of machines
 */
size_t FlowshopBasic::getTotalMachines()
{
    return ptMatrixRows;
}

/**
 * @brief Returns the number of times the current flowshop objective function has been executed
 * 
 * @return Returns the number of times the current flowshop objective function has been executed
 */
size_t FlowshopBasic::getFuncCallCounts()
{
    return funcCallCounter;
}

/**
 * @brief Calculates the objective flowshop scheduling problem result using the given
 * job sequence.
 * 
 * @param seq Pointer to an int array containing the job sequence permutation
 * @param seqSize Size of the job sequence array
 * @return Returns a unique_ptr to a FlowshopSolution object that contains all solution results
 */
std::unique_ptr<FlowshopSolution> FlowshopBasic::calcObjective(int* seq, size_t seqSize)
{
    // Validate input parameters
    validateParams(seq, seqSize);

    // Allocate completion (departure) time matrix and start time matrix
    auto compTimeMatrix = allocTimeMatrix(ptMatrixRows, seqSize);
    startTimeMatrix = allocTimeMatrix(ptMatrixRows, seqSize);

    // Initialize completion time matrix and start time matrix
    initTimeMatrix(compTimeMatrix, seq, ptMatrixRows, seqSize);
    calcStartTimeCol(startTimeMatrix, compTimeMatrix, seq, 0, ptMatrixRows, seqSize);

    // Calculate all completion and start times
    calcTimeMatrix(compTimeMatrix, seq, ptMatrixRows, seqSize);

    // Construct solution struct
    auto retVal = std::unique_ptr<FlowshopSolution>(new FlowshopSolution(startTimeMatrix, compTimeMatrix, ptMatrixRows, seq, seqSize, 
        getCmax(compTimeMatrix, ptMatrixRows, seqSize), getTFT(compTimeMatrix, ptMatrixRows, seqSize)));

    // Increment obj func call counter and return result
    funcCallCounter += 1;
    return std::move(retVal);
}

/**
 * @brief Validates the flowshop input parameters, and throws an exception on error
 * 
 * @param seq Job permutation sequence array
 * @param seqSize Size of job sequence array
 */
void FlowshopBasic::validateParams(int* seq, size_t seqSize)
{
    // Make sure job sequence is not empty, or too large
    if (seqSize == 0 || seqSize > ptMatrixCols)
    {
        std::string msg = "Error: seqSize cannot be larger than ptMatrixCols";
        throw std::out_of_range(msg);
    }

    // Make sure all jobs in job sequence are within bounds of processing time matrix
    for (size_t i = 0; i < seqSize; i++)
    {
        if (seq[i] <= 0 || seq[i] > ptMatrixCols)
        {
            std::string msg = "Error: seq contains a job number out of range [1, ";
            msg += std::to_string(ptMatrixCols);
            msg += "]";
            throw std::out_of_range(msg);
        }
    }
}

/**
 * @brief Allocates the start times and completion time matrices
 * 
 * @param rows Number of rows (machines)
 * @param cols Number of columns (jobs)
 * @return Returns a pointer to the newly created matrix
 */
int** FlowshopBasic::allocTimeMatrix(size_t rows, size_t cols)
{
    int** timeMatrix = util::allocMatrix<int>(rows, cols);
    if (timeMatrix == nullptr)
    {
        std::cerr << "Error allocating time matrix." << std::endl;
        throw std::bad_alloc();
    }

    util::initMatrix<int>(timeMatrix, rows, cols, 0);

    return timeMatrix;
}

/**
 * @brief Initializes the completion time matrix (first row and first column)
 * so that it is ready to be completed with the main algorithm.
 * 
 * @param compTimeMatrix Pointer to completion time matrix
 * @param seq Pointer to job sequence
 * @param rows Number of rows (machines) in the completion time matrix
 * @param cols Number of columns (jobs) in the completion time matrix
 */
void FlowshopBasic::initTimeMatrix(int** compTimeMatrix, int* seq, size_t rows, size_t cols)
{
    // Set first job, first machine
    compTimeMatrix[0][0] = procTimeMatrix[0][seq[0] - 1];

    // Set first job for all machines
    for (size_t r = 1; r < rows; r++)
    {
        compTimeMatrix[r][0] = procTimeMatrix[r][seq[0] - 1] + compTimeMatrix[r - 1][0];
    }

    // Set first machine for all jobs
    for (size_t c = 1; c < cols; c++)
    {
        compTimeMatrix[0][c] = compTimeMatrix[0][c - 1] + procTimeMatrix[0][seq[c] - 1];
    }
}

/**
 * @brief Calculates all remaining start and completion times for the current flowshop problem.
 * 
 * @param compTimeMatrix Pointer to completion time matrix
 * @param seq Pointer to job sequence
 * @param rows Number of rows (machines) in the completion time matrix
 * @param cols Number of columns (jobs) in the completion time matrix
 */
void FlowshopBasic::calcTimeMatrix(int** compTimeMatrix, int* seq, size_t rows, size_t cols)
{
    for (size_t c = 1; c < cols; c++)
    {
        for (size_t r = 1; r < rows; r++)
        {
            int c1 = compTimeMatrix[r - 1][c];
            int c2 = compTimeMatrix[r][c - 1];

            compTimeMatrix[r][c] = max(c1, c2) + procTimeMatrix[r][seq[c] - 1];
        }

        FlowshopBasic::calcStartTimeCol(startTimeMatrix, compTimeMatrix, seq, c, rows, cols);
    }
}

/**
 * @brief Calculates the start times for a single column. Depends on values in completion time matrix.
 * 
 * @param startTimeMatrix Pointer to start times matrix
 * @param departTimeMatrix Pointer to departure (completion) times matrix
 * @param seq Pointer to job sequence
 * @param curCol Index of the column to be calculated
 * @param rows Number of rows (machines) in the completion time matrix
 * @param cols Number of columns (jobs) in the completion time matrix
 */
void FlowshopBasic::calcStartTimeCol(int** startTimeMatrix, int** departTimeMatrix, int* seq, size_t curCol, size_t rows, size_t cols)
{
    for (size_t r = rows; r > 0; r--)
    {
        startTimeMatrix[r - 1][curCol] = departTimeMatrix[r - 1][curCol] - procTimeMatrix[r - 1][seq[curCol] - 1];
    }
}

/**
 * @brief Returns the cmax value for a given completion time matrix
 * 
 * @param compTimeMatrix Pointer to the completion time matrix
 * @param rows Number of rows (machines) in the completion time matrix
 * @param cols Number of columns (jobs) in the completion time matrix
 * @return Returns the cmax value (last row, last column) in the completion time matrix
 */
int FlowshopBasic::getCmax(int** compTimeMatrix, size_t rows, size_t cols)
{
    return compTimeMatrix[rows - 1][cols - 1];
}

/**
 * @brief Returns the total flow time value for a given completion time matrix
 * 
 * @param compTimeMatrix Pointer to the completion time matrix
 * @param rows Number of rows (machines) in the completion time matrix
 * @param cols Number of columns (jobs) in the completion time matrix
 * @return Returns the TFT value (sum of last row) in the completion time matrix
 */
int FlowshopBasic::getTFT(int** compTimeMatrix, size_t rows, size_t cols)
{
    int sum = 0;

    for (size_t c = 0; c < cols; c++)
    {
        sum += compTimeMatrix[rows - 1][c];
    }

    return sum;
}

// =========================
// End of flowshopbasic.cpp
// =========================
