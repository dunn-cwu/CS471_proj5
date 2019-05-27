#ifndef __FLOWSHOPBASIC_H
#define __FLOWSHOPBASIC_H

#include <stddef.h>
#include <stdexcept>
#include <memory>

namespace fshop
{
    struct FlowshopSolution
    {
        FlowshopSolution(int** _timeMatrix, size_t _tMatrixRows, int* _jobSeq, size_t _seqSize, int _cmax, int _totalFlowTime);
        ~FlowshopSolution();

        const size_t seqSize;
        const size_t numMachines;
        const int cmax;
        const int totalFlowTime;

        const int* const getJobSeq();
        const int** const getTimeMatrix();

        // Copy constructor
        FlowshopSolution(const FlowshopSolution& obj);

        // Move constructor
        FlowshopSolution(FlowshopSolution&& obj);

        // Delete move assignment
        FlowshopSolution& operator=(FlowshopSolution&& obj) = delete;

        // Delete copy assignment
        FlowshopSolution& operator=(const FlowshopSolution& obj) = delete;

    private:
        int* jobSequence;
        int** timeMatrix;
    };

    class FlowshopBasic
    {
    public:
        FlowshopBasic(const char* procTimeMatrixFile);
        virtual ~FlowshopBasic();
        virtual std::unique_ptr<FlowshopSolution> calcObjective(int* seq, size_t seqSize);

        virtual size_t getTotalJobs();
        virtual size_t getTotalMachines();
    protected:
        int** procTimeMatrix;
        size_t ptMatrixRows;
        size_t ptMatrixCols;
        size_t funcCallCounter;

        virtual void initTimeMatrix(int** compTimeMatrix, int* seq, size_t rows, size_t cols);
        virtual void calcTimeMatrix(int** compTimeMatrix, int* seq, size_t rows, size_t cols);
        virtual int getCmax(int** compTimeMatrix, size_t rows, size_t cols);
        virtual int getTFT(int** compTimeMatrix, size_t rows, size_t cols);
    };
}

#endif