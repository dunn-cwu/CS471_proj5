#ifndef __FLOWSHOPBASIC_H
#define __FLOWSHOPBASIC_H

#include <stddef.h>
#include <stdexcept>
#include <memory>

namespace fshop
{
    struct FlowshopSolution
    {
        FlowshopSolution(int** _startimeMatrix, int** _departTimeMatrix, size_t _tMatrixRows, int* _jobSeq, size_t _seqSize, int _cmax, int _totalFlowTime);
        ~FlowshopSolution();

        const size_t seqSize;
        const size_t numMachines;
        const int cmax;
        const int totalFlowTime;

        const int* const getJobSeq();
        const int** const getStartTimeMatrix();
        const int** const getDepartTimeMatrix();

        // Copy constructor
        FlowshopSolution(const FlowshopSolution& obj);

        // Move constructor
        FlowshopSolution(FlowshopSolution&& obj);

        // Delete copy assignment
        FlowshopSolution& operator=(const FlowshopSolution& obj) = delete;

        // Delete move assignment
        FlowshopSolution& operator=(FlowshopSolution&& obj) = delete;
    private:
        int* jobSequence;
        int** startTimeMatrix;
        int** departTimeMatrix;
    };

    class FlowshopBasic
    {
    public:
        FlowshopBasic(const char* procTimeMatrixFile);
        virtual ~FlowshopBasic();
        virtual std::unique_ptr<FlowshopSolution> calcObjective(int* seq, size_t seqSize);

        virtual int getProcessingTime(size_t machine, size_t job);
        virtual size_t getTotalJobs();
        virtual size_t getTotalMachines();

        // Delete copy/move constructors and assignments
        FlowshopBasic(const FlowshopBasic& o) = delete;
        FlowshopBasic(const FlowshopBasic&& o) = delete;
        FlowshopBasic& operator=(const FlowshopBasic& o) = delete;
        FlowshopBasic& operator=(const FlowshopBasic&& o) = delete;
    protected:
        int** procTimeMatrix;
        int** startTimeMatrix;
        size_t ptMatrixRows;
        size_t ptMatrixCols;
        size_t funcCallCounter;

        virtual void validateParams(int* seq, size_t seqSize);
        virtual int** allocTimeMatrix(size_t rows, size_t cols);
        virtual void initTimeMatrix(int** compTimeMatrix, int* seq, size_t rows, size_t cols);
        virtual void calcTimeMatrix(int** compTimeMatrix, int* seq, size_t rows, size_t cols);
        virtual void calcStartTimeCol(int** startTimeMatrix, int** departTimeMatrix, int* seq, size_t curCol, size_t rows, size_t cols);
        virtual int getCmax(int** compTimeMatrix, size_t rows, size_t cols);
        virtual int getTFT(int** compTimeMatrix, size_t rows, size_t cols);
    };
}

#endif