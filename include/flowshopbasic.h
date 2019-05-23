#ifndef __FLOWSHOPBASIC_H
#define __FLOWSHOPBASIC_H

#include <stddef.h>
#include <stdexcept>

namespace fshop
{
    struct FlowshopSolution
    {
        FlowshopSolution(size_t numJobs);
        ~FlowshopSolution();

        int cmax;
        int totalFlowTime;

        int& operator[](size_t index);

        // Move constructor and Move assignment
        FlowshopSolution(FlowshopSolution&& obj);
        FlowshopSolution& operator=(FlowshopSolution&& obj);

        // Delete copy constructor and copy assignment
        FlowshopSolution(const FlowshopSolution& obj) = delete;
        FlowshopSolution& operator=(const FlowshopSolution& obj) = delete;

    private:
        int* jobSequence;
        size_t seqSize;
    };

    class FlowshopBasic
    {
    public:
        FlowshopBasic();
        virtual FlowshopSolution calcObjective(int* seq, size_t seqSize);
    protected:
        size_t funcCallCounter;
    };
}

#endif