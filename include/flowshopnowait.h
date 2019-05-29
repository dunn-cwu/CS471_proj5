/**
 * @file flowshopnowait.h
 * @author Andrew Dunn (Andrew.Dunn@cwu.edu)
 * @brief Contains the FlowshopNoWait class, which inherits
 * FlowshopBasic and solves a flowshop with no waiting problem
 * for a specific job sequence.
 * @version 0.1
 * @date 2019-05-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef __FLOWSHOPNOWAIT_H
#define __FLOWSHOPNOWAIT_H

#include "flowshopbasic.h"

namespace fshop
{
    /**
     * @brief The FlowshopNoWait class runs the flowshop with no wait problem
     * for a given job-machine processing time matrix that is read from a file. The run()
     * method takes the specific job sequence being calculated. Inherits from FlowshopBasic.
     */
    class FlowshopNoWait : public fshop::FlowshopBasic
    {
    public:
        FlowshopNoWait(const char* procTimeMatrixFile);
        virtual ~FlowshopNoWait() = default;
    protected:
        virtual void initTimeMatrix(int** departTimeMatrix, int* seq, size_t rows, size_t cols) override;
        virtual void calcTimeMatrix(int** departTimeMatrix, int* seq, size_t rows, size_t cols) override;
    };
}

#endif

// =========================
// End of flowshopnowait.h
// =========================
