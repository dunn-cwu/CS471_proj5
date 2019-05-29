/**
 * @file neh.h
 * @author Andrew Dunn (Andrew.Dunn@cwu.edu)
 * @brief Contains the NEH class, which runs the NEH algorithm on
 * a given flowshop problem. The NEH algorithm aims to optimize the
 * job sequence such that it produces the smallest cMax value.
 * @version 0.1
 * @date 2019-05-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef __NEH_H
#define __NEH_H

#include <list>
#include <iostream>
#include <random>
#include "flowshopbasic.h"

using fsSol = std::unique_ptr<fshop::FlowshopSolution>;

namespace fshop
{
    /**
     * @brief Simple struct that pairs a job with it's
     * total processing time. Used for sorting purposes.
     */
    struct JobTimePair
    {
        const int job;
        const int time;

        JobTimePair(int _job, int _time)
            : job(_job), time(_time)
        {
        }
    };

    /**
     * @brief The NEH class runs the NEH algorithm on the given flowshop
     * objective function and attempts to optimize the job sequence that
     * produces the smallest cmax value.
     * 
     */
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

// =========================
// End of neh.h
// =========================
