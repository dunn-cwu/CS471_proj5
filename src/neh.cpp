/**
 * @file neh.cpp
 * @author Andrew Dunn (Andrew.Dunn@cwu.edu)
 * @brief Implementation file for the NEH class.
 * @version 0.1
 * @date 2019-05-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "neh.h"

// Type alias
using jtList = std::list<fshop::JobTimePair>;
using jList = std::list<int>;

/**
 * @brief Construct a new NEH object
 * 
 */
fshop::NEH::NEH()
    : rd(), randEngine(rd()), randChance(0, 1)
{ }

/**
 * @brief Runs the NEH algorithm on the given flowshop objective function
 * 
 * @param objectiveFs Pointer to the flowshop objective function being optimized
 * @return Returns a unique_ptr to a FlowshopSolution object that contains the best solution found.
 */
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

/**
 * @brief Generates the initial jobs list and sorts them by processing time in descending order
 * 
 * @param objectiveFs Pointer to the flowshop objective function being optimized
 * @param outList Out reference to the job list that will be filled and sorted
 */
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

/**
 * @brief Finds the best permutation of an existing job sequence and an additional inserted job.
 * 
 * @param objectiveFs Pointer to the flowshop objective function being optimized
 * @param baseList Base job sequence list
 * @param jobInsert Job that is being inserted
 * @param outBestSeq Out reference list that will be filled with the best job sequence found
 * @return Returns a unique_ptr to the FlowshopSolution object created from the best job sequence
 */
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
        if (bestSol == nullptr || result->cmax < bestSol->cmax ||
            (result->cmax == bestSol->cmax && randChance(randEngine) >= 0.5))
        {
            bestSol.reset();
            bestSol = std::move(result);
            outBestSeq = jList(bufferList.begin(), bufferList.end());
        }
    }

    delete[] seqArr;
    return std::move(bestSol);
}

// =========================
// End of neh.cpp
// =========================
