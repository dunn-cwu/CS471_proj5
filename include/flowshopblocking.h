#ifndef __FLOWSHOPBLOCKING_H
#define __FLOWSHOPBLOCKING_H

#include "flowshopbasic.h"

namespace fshop
{
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