/**
 * @file flowshopblocking.cpp
 * @author Andrew Dunn (Andrew.Dunn@cwu.edu)
 * @brief Implementation file for the FlowshopBlocking class
 * @version 0.1
 * @date 2019-05-25
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "flowshopblocking.h"

using namespace fshop;

/**
 * @brief Simple inline helper function that returns the max of two integers
 * 
 * @param val1 First integer
 * @param val2 Second integer
 * @return Returns the maximum of the two integers
 */
inline int max(int val1, int val2)
{
    if (val1 >= val2) return val1;
    else return val2;
}

/**
 * @brief Construct a new FlowshopBlocking object
 * 
 * @param procTimeMatrixFile File path to the file containing the job processing times matrix
 */
FlowshopBlocking::FlowshopBlocking(const char* procTimeMatrixFile)
    : FlowshopBasic(procTimeMatrixFile)
{
}

/**
 * @brief Initializes the completion time matrix (first column)
 * so that it is ready to be completed with the main algorithm.
 * Overrides method in base class.
 * 
 * @param compTimeMatrix Pointer to completion time matrix
 * @param seq Pointer to job sequence
 * @param rows Number of rows (machines) in the completion time matrix
 * @param cols Number of columns (jobs) in the completion time matrix
 */
void FlowshopBlocking::initTimeMatrix(int** departTimeMatrix, int* seq, size_t rows, size_t cols)
{
    departTimeMatrix[0][0] = procTimeMatrix[0][seq[0] - 1];

    for (size_t r = 1; r < rows; r++)
    {
        departTimeMatrix[r][0] = procTimeMatrix[r][seq[0] - 1] + departTimeMatrix[r - 1][0];
    }
}

/**
 * @brief Calculates all remaining start and completion times for the current flowshop problem.
 * Overrides method in base class.
 * 
 * @param compTimeMatrix Pointer to completion time matrix
 * @param seq Pointer to job sequence
 * @param rows Number of rows (machines) in the completion time matrix
 * @param cols Number of columns (jobs) in the completion time matrix
 */
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

        FlowshopBasic::calcStartTimeCol(startTimeMatrix, departTimeMatrix, seq, c, rows, cols);
    }
}

// =========================
// End of flowshopblocking.cpp
// =========================
