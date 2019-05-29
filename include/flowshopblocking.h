/**
 * @file flowshopblocking.h
 * @author Andrew Dunn (Andrew.Dunn@cwu.edu)
 * @brief Contains the FlowshopBlocking class, which inherits
 * FlowshopBasic and solves a flowshop with blocking problem
 * for a specific job sequence.
 * @version 0.1
 * @date 2019-05-25
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef __FLOWSHOPBLOCKING_H
#define __FLOWSHOPBLOCKING_H

#include "flowshopbasic.h"

namespace fshop
{
    /**
     * @brief The FlowshopBlocking class runs the flowshop with blocking problem
     * for a given job-machine processing time matrix that is read from a file. The run()
     * method takes the specific job sequence being calculated. Inherits from FlowshopBasic.
     */
    class FlowshopBlocking : public fshop::FlowshopBasic
    {
    public:
        FlowshopBlocking(const char* procTimeMatrixFile);
        virtual ~FlowshopBlocking() = default;
    protected:
        virtual void initTimeMatrix(int** compTimeMatrix, int* seq, size_t rows, size_t cols) override;
        virtual void calcTimeMatrix(int** compTimeMatrix, int* seq, size_t rows, size_t cols) override;
    };
}

#endif

// =========================
// End of flowshopblocking.h
// =========================
