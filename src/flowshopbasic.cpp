#include "flowshopbasic.h"

using namespace fshop;

// ============================================================

FlowshopSolution::FlowshopSolution(size_t numJobs)
    : jobSequence(new int[numJobs]), seqSize(numJobs)
{
}

FlowshopSolution::~FlowshopSolution()
{
    if (jobSequence != nullptr)
    {
        delete[] jobSequence;
        jobSequence = nullptr;
    }
}

int& FlowshopSolution::operator[](size_t index)
{
    if (jobSequence == nullptr || index > seqSize || index == 0)
        throw std::out_of_range("Flowshop sequence index out of range.");

    return jobSequence[index - 1];
}

FlowshopSolution::FlowshopSolution(FlowshopSolution&& obj)
{
    seqSize = obj.seqSize;
    cmax = obj.cmax;
    totalFlowTime = obj.totalFlowTime;
    jobSequence = obj.jobSequence;
    obj.jobSequence = nullptr;
}

 FlowshopSolution& FlowshopSolution::operator=(FlowshopSolution&& obj)
{
    seqSize = obj.seqSize;
    cmax = obj.cmax;
    totalFlowTime = obj.totalFlowTime;
    jobSequence = obj.jobSequence;
    obj.jobSequence = nullptr;
}

// ============================================================

FlowshopBasic::FlowshopBasic()
    : funcCallCounter(0)
{
}

FlowshopSolution FlowshopBasic::calcObjective(int* seq, size_t seqSize)
{
    
}