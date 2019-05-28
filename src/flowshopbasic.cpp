#include <stdexcept>
#include <fstream>
#include "flowshopbasic.h"
#include "mem.h"

using namespace fshop;

inline int max(int val1, int val2)
{
    if (val1 >= val2) return val1;
    else return val2;
}

// ============================================================

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

FlowshopSolution::~FlowshopSolution()
{
    util::releaseArray<int>(jobSequence);
    util::releaseMatrix<int>(startTimeMatrix, numMachines);
    util::releaseMatrix<int>(departTimeMatrix, numMachines);
}

const int* const FlowshopSolution::getJobSeq()
{
    return const_cast<const int* const>(jobSequence);
}

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

const int** const FlowshopSolution::getStartTimeMatrix()
{
    return const_cast<const int** const>(startTimeMatrix);
}

const int** const FlowshopSolution::getDepartTimeMatrix()
{
    return const_cast<const int** const>(departTimeMatrix);
}

bool FlowshopSolution::outputTimesCsv(const std::string& fileNamePrefix)
{
    using namespace std;

    string startTimesFile = fileNamePrefix + "starttimes.csv";
    string departTimesFile = fileNamePrefix + "departtimes.csv";

    ofstream startOs = ofstream(startTimesFile, ios::trunc | ios::out);
    if (!startOs.good()) return false;

    ofstream departOs = ofstream(departTimesFile, ios::trunc | ios::out);
    if (!departOs.good()) return false;

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

    startOs.close();
    departOs.close();
    return true;
}

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

FlowshopBasic::FlowshopBasic(const char* procTimeMatrixFile)
    : startTimeMatrix(nullptr), ptMatrixRows(0), ptMatrixCols(0), funcCallCounter(0)
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

size_t FlowshopBasic::getTotalJobs()
{
    return ptMatrixCols;
}

size_t FlowshopBasic::getTotalMachines()
{
    return ptMatrixRows;
}

std::unique_ptr<FlowshopSolution> FlowshopBasic::calcObjective(int* seq, size_t seqSize)
{
    validateParams(seq, seqSize);

    auto compTimeMatrix = allocTimeMatrix(ptMatrixRows, seqSize);
    startTimeMatrix = allocTimeMatrix(ptMatrixRows, seqSize);

    initTimeMatrix(compTimeMatrix, seq, ptMatrixRows, seqSize);
    calcStartTimeCol(startTimeMatrix, compTimeMatrix, seq, 0, ptMatrixRows, seqSize);

    calcTimeMatrix(compTimeMatrix, seq, ptMatrixRows, seqSize);

    auto retVal = std::unique_ptr<FlowshopSolution>(new FlowshopSolution(startTimeMatrix, compTimeMatrix, ptMatrixRows, seq, seqSize, 
        getCmax(compTimeMatrix, ptMatrixRows, seqSize), getTFT(compTimeMatrix, ptMatrixRows, seqSize)));

    funcCallCounter += 1;
    return std::move(retVal);
}

void FlowshopBasic::validateParams(int* seq, size_t seqSize)
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
}

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

        FlowshopBasic::calcStartTimeCol(startTimeMatrix, compTimeMatrix, seq, c, rows, cols);
    }
}

void FlowshopBasic::calcStartTimeCol(int** startTimeMatrix, int** departTimeMatrix, int* seq, size_t curCol, size_t rows, size_t cols)
{
    for (size_t r = rows; r > 0; r--)
    {
        startTimeMatrix[r - 1][curCol] = departTimeMatrix[r - 1][curCol] - procTimeMatrix[r - 1][seq[curCol] - 1];
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
