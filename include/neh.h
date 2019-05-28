#ifndef __NEH_H
#define __NEH_H

#include <list>
#include <iostream>
#include <random>
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
        NEH();
        fsSol run(FlowshopBasic* const objectiveFs);
    private:
        std::random_device rd;
        std::mt19937 randEngine;
        std::uniform_real_distribution<float> randChance;

        void makeInitialAvailJobList(FlowshopBasic* const objectiveFs, std::list<fshop::JobTimePair>& outList);
        fsSol bestPermutation(FlowshopBasic* const objectiveFs, const std::list<int>& baseList, int jobInsert, std::list<int>& outBestSeq);
    };
}

#endif