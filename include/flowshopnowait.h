#ifndef __FLOWSHOPNOWAIT_H
#define __FLOWSHOPNOWAIT_H

#include "flowshopbasic.h"

namespace fshop
{
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