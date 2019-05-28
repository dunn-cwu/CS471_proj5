#include <iostream>
#include <mem.h>
#include "flowshopnowait.h"
#include "mem.h"

using namespace fshop;

inline int max(int val1, int val2)
{
    if (val1 >= val2) return val1;
    else return val2;
}

FlowshopNoWait::FlowshopNoWait(const char* procTimeMatrixFile)
    : FlowshopBasic(procTimeMatrixFile)
{
}

void FlowshopNoWait::initTimeMatrix(int** departTimeMatrix, int* seq, size_t rows, size_t cols)
{
    departTimeMatrix[0][0] = procTimeMatrix[0][seq[0] - 1];

    for (size_t r = 1; r < rows; r++)
    {
        departTimeMatrix[r][0] = procTimeMatrix[r][seq[0] - 1] + departTimeMatrix[r - 1][0];
    }
}

void FlowshopNoWait::calcTimeMatrix(int** departTimeMatrix, int* seq, size_t rows, size_t cols)
{
    for (size_t c = 1; c < cols; c++)
    {
        departTimeMatrix[0][c] = departTimeMatrix[0][c - 1] + procTimeMatrix[0][seq[c] - 1];

        for (size_t r = 1; r < rows; r++)
        {
            int d1 = departTimeMatrix[r - 1][c];
            int d2 = departTimeMatrix[r][c - 1];

            if (d1 < d2)
            {
                const int diff = d2 - d1;
                for (size_t r2 = r + 1; r2 > 0; r2--)
                    departTimeMatrix[r2 - 1][c] += diff;

                d1 = departTimeMatrix[r - 1][c];
            }

            departTimeMatrix[r][c] = d1 + procTimeMatrix[r][seq[c] - 1];
        }

        FlowshopBasic::calcStartTimeCol(startTimeMatrix, departTimeMatrix, seq, c, rows, cols);
    }
}