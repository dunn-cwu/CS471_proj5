#ifndef __NEH_H
#define __NEH_H

#include <list>
#include <iostream>
#include "flowshopbasic.h"

using fsSol = std::unique_ptr<fshop::FlowshopSolution>;

namespace fshop
{
    struct JobTimePair
    {
        const int job;
        const int time;

        JobTimePair(int _job, int _time)
            : job(_job), time(_time)
        {
        }
    };

    class NEH
    {
    public:
        static fsSol run(FlowshopBasic* const objectiveFs);
    private:
        static void makeInitialAvailJobList(FlowshopBasic* const objectiveFs, std::list<fshop::JobTimePair>& outList);
        static fsSol bestPermutation(FlowshopBasic* const objectiveFs, const std::list<int>& baseList, int jobInsert, std::list<int>& outBestSeq);
    };
}

using jtList = std::list<fshop::JobTimePair>;
using jList = std::list<int>;

fsSol fshop::NEH::run(FlowshopBasic* const objectiveFs)
{
    jtList availJobsList;
    makeInitialAvailJobList(objectiveFs, availJobsList);

    auto firstJob = availJobsList.front();
    availJobsList.pop_front();

    fsSol bestSol = nullptr;
    jList* curJobSeq = new jList();
    jList* nextJobSeq = new jList();
    curJobSeq->push_back(firstJob.job);

    while (availJobsList.size() > 0)
    {
        auto nextJob = availJobsList.front();
        availJobsList.pop_front();

        bestSol.reset();
        bestSol = bestPermutation(objectiveFs, *curJobSeq, nextJob.job, *nextJobSeq);

        auto tmp = curJobSeq;
        curJobSeq = nextJobSeq;
        nextJobSeq = tmp;
    }

    delete curJobSeq;
    delete nextJobSeq;

    return std::move(bestSol);
}

void fshop::NEH::makeInitialAvailJobList(FlowshopBasic* const objectiveFs, jtList& outList)
{
    const size_t numMachines = objectiveFs->getTotalMachines();
    const size_t numJobs = objectiveFs->getTotalJobs();
    
    for (size_t j = 1; j <= numJobs; j++)
    {
        int sum = 0;
        for (size_t m = 1; m <= numMachines; m++)
            sum += objectiveFs->getProcessingTime(m, j);
        
        outList.emplace_back(
            JobTimePair(j, sum)
        );
    }

    // Sort in decreasing order of time
    outList.sort([](JobTimePair& lhs, JobTimePair& rhs) { return lhs.time >= rhs.time; });
}

fsSol fshop::NEH::bestPermutation(FlowshopBasic* const objectiveFs, const jList& baseList, int jobInsert, jList& outBestSeq)
{
    fsSol bestSol = nullptr;
    outBestSeq.clear();

    jList bufferList;
    int* seqArr = new int[baseList.size() + 1];

    for (size_t i = 0; i <= baseList.size(); i++)
    {
        bufferList = jList(baseList.begin(), baseList.end());
        auto it = bufferList.begin();
        std::advance(it, i);

        bufferList.insert(it, jobInsert);
        
        size_t index = 0;

        for (auto i = bufferList.begin(); i != bufferList.end(); i++)
        {
            seqArr[index] = *i;
            index++;
        }

        auto result = objectiveFs->calcObjective(seqArr, bufferList.size());
        if (bestSol == nullptr || result->cmax < bestSol->cmax)
        {
            bestSol.reset();
            bestSol = std::move(result);
            outBestSeq = jList(bufferList.begin(), bufferList.end());
        }
    }

    delete[] seqArr;
    return std::move(bestSol);
}

#endif