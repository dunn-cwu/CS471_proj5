#include <stdexcept>
#include "flowshopbasic.h"
#include "mem.h"

using namespace fshop;

inline int max(int val1, int val2)
{
    if (val1 >= val2) return val1;
    else return val2;
}

// ============================================================

FlowshopSolution::FlowshopSolution(int** _timeMatrix, size_t _tMatrixRows, int* _jobSeq, size_t _seqSize, int _cmax, int _totalFlowTime)
    : timeMatrix(_timeMatrix), numMachines(_tMatrixRows), seqSize(_seqSize), cmax(_cmax), totalFlowTime(_totalFlowTime)
{
    if (_jobSeq == nullptr)
        throw std::invalid_argument("Error: _jobSeq cannot be nullptr");
    else if (_timeMatrix == nullptr)
        throw std::invalid_argument("Error: _timeMatrix cannot be nullptr");
    else if (seqSize == 0)
        throw std::invalid_argument("Error: _seqSize cannot be zero");

    jobSequence = util::allocArray<int>(seqSize);
    for (size_t i = 0; i < seqSize; i++)
        jobSequence[i] = _jobSeq[i];
}

FlowshopSolution::~FlowshopSolution()
{
    util::releaseArray<int>(jobSequence);
    util::releaseMatrix<int>(timeMatrix, numMachines);
}

const int* const FlowshopSolution::getJobSeq()
{
    return const_cast<const int* const>(jobSequence);;
}

const int** const FlowshopSolution::getTimeMatrix()
{
    return const_cast<const int** const>(timeMatrix);
}

FlowshopSolution::FlowshopSolution(const FlowshopSolution& obj)
    : timeMatrix(obj.timeMatrix), numMachines(obj.numMachines), seqSize(obj.seqSize), cmax(obj.cmax), totalFlowTime(obj.totalFlowTime)
{
    if (obj.jobSequence == nullptr)
        throw std::invalid_argument("Error: jobSequence cannot be nullptr");
    else if (seqSize == 0)
        throw std::invalid_argument("Error: seqSize cannot be zero");

    jobSequence = util::allocArray<int>(seqSize);
    for (size_t i = 0; i < seqSize; i++)
        jobSequence[i] = obj.jobSequence[i];
}

FlowshopSolution::FlowshopSolution(FlowshopSolution&& obj)
    : numMachines(obj.numMachines), seqSize(obj.seqSize), cmax(obj.cmax), totalFlowTime(obj.totalFlowTime)
{
    jobSequence = obj.jobSequence;
    timeMatrix = obj.timeMatrix;
    obj.jobSequence = nullptr;
    obj.timeMatrix = nullptr;
}

// ============================================================

FlowshopBasic::FlowshopBasic(const char* procTimeMatrixFile)
    : ptMatrixRows(0), ptMatrixCols(0), funcCallCounter(0)
{
    procTimeMatrix = util::loadMatrixFromFile<int>(procTimeMatrixFile, ptMatrixRows, ptMatrixCols);
    if (procTimeMatrix == nullptr)
    {
        std::string msg = "Error when loading matrix file: ";
        msg += procTimeMatrixFile;
        throw std::runtime_error(msg);
    }
}

FlowshopBasic::~FlowshopBasic()
{
    util::releaseMatrix<int>(procTimeMatrix, ptMatrixRows);
}

size_t FlowshopBasic::getTotalJobs()
{
    return ptMatrixRows;
}

size_t FlowshopBasic::getTotalMachines()
{
    return ptMatrixCols;
}

std::unique_ptr<FlowshopSolution> FlowshopBasic::calcObjective(int* seq, size_t seqSize)
{
    if (seqSize == 0 || seqSize > ptMatrixCols)
    {
        std::string msg = "Error: seqSize cannot be larger than ptMatrixCols";
        throw std::out_of_range(msg);
    }

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

    int** compTimeMatrix = util::allocMatrix<int>(ptMatrixRows, seqSize);
    if (compTimeMatrix == nullptr)
    {
        std::cerr << "Error allocating completion time matrix." << std::endl;
        throw std::bad_alloc();
    }

    util::initMatrix<int>(compTimeMatrix, ptMatrixRows, seqSize, 0);

    initTimeMatrix(compTimeMatrix, seq, ptMatrixRows, seqSize);
    util::outputMatrix(std::cout, compTimeMatrix, ptMatrixRows, seqSize);
    std::cout << std::endl;

    calcTimeMatrix(compTimeMatrix, seq, ptMatrixRows, seqSize);
    util::outputMatrix(std::cout, compTimeMatrix, ptMatrixRows, seqSize);
    std::cout << std::endl;

    auto retVal = std::unique_ptr<FlowshopSolution>(new FlowshopSolution(compTimeMatrix, ptMatrixRows, seq, seqSize, 
        getCmax(compTimeMatrix, ptMatrixRows, seqSize), getTFT(compTimeMatrix, ptMatrixRows, seqSize)));

    return std::move(retVal);
}

void FlowshopBasic::initTimeMatrix(int** compTimeMatrix, int* seq, size_t rows, size_t cols)
{
    compTimeMatrix[0][0] = procTimeMatrix[0][seq[0] - 1];

    for (size_t r = 1; r < rows; r++)
    {
        compTimeMatrix[r][0] = procTimeMatrix[r][seq[0] - 1] + compTimeMatrix[r - 1][0];
    }

    for (size_t c = 1; c < cols; c++)
    {
        compTimeMatrix[0][c] = compTimeMatrix[0][c - 1] + procTimeMatrix[0][seq[c] - 1];
    }
}

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
    }
}

int FlowshopBasic::getCmax(int** compTimeMatrix, size_t rows, size_t cols)
{
    return compTimeMatrix[rows - 1][cols - 1];
}

int FlowshopBasic::getTFT(int** compTimeMatrix, size_t rows, size_t cols)
{
    int sum = 0;

    for (size_t c = 0; c < cols; c++)
    {
        sum += compTimeMatrix[rows - 1][c];
    }

    return sum;
}
