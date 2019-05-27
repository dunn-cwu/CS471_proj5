#include "flowshopblocking.h"

using namespace fshop;

inline int max(int val1, int val2)
{
    if (val1 >= val2) return val1;
    else return val2;
}

FlowshopBlocking::FlowshopBlocking(const char* procTimeMatrixFile)
    : FlowshopBasic(procTimeMatrixFile)
{
}

void FlowshopBlocking::initTimeMatrix(int** departTimeMatrix, int* seq, size_t rows, size_t cols)
{
    departTimeMatrix[0][0] = procTimeMatrix[0][seq[0] - 1];

    for (size_t r = 1; r < rows; r++)
    {
        departTimeMatrix[r][0] = procTimeMatrix[r][seq[0] - 1] + departTimeMatrix[r - 1][0];
    }
}

void FlowshopBlocking::calcTimeMatrix(int** departTimeMatrix, int* seq, size_t rows, size_t cols)
{
    for (size_t c = 1; c < cols; c++)
    {
        int d1 = departTimeMatrix[0][c - 1] + procTimeMatrix[0][seq[c] - 1];
        int d2 = departTimeMatrix[1][c - 1];

        departTimeMatrix[0][c] = max(d1, d2);

        for (size_t r = 1; r < rows - 1; r++)
        {
            int d1 = departTimeMatrix[r - 1][c] + procTimeMatrix[r][seq[c] - 1];
            int d2 = departTimeMatrix[r + 1][c - 1];

            departTimeMatrix[r][c] = max(d1, d2);
        }

        departTimeMatrix[rows - 1][c] = departTimeMatrix[rows - 2][c] + procTimeMatrix[rows - 1][seq[c] - 1];
    }
}