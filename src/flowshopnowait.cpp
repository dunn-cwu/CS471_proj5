/**
 * @file flowshopnowait.cpp
 * @author Andrew Dunn (Andrew.Dunn@cwu.edu)
 * @brief Implementation file for the FlowshopNoWait class.
 * @version 0.1
 * @date 2019-05-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <iostream>
#include <mem.h>
#include "flowshopnowait.h"
#include "mem.h"

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
 * @brief Construct a new FlowshopNoWait object
 * 
 * @param procTimeMatrixFile File path to the file containing the job processing times matrix
 */
FlowshopNoWait::FlowshopNoWait(const char* procTimeMatrixFile)
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
void FlowshopNoWait::initTimeMatrix(int** departTimeMatrix, int* seq, size_t rows, size_t cols)
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

// =========================
// End of flowshopnowait.cpp
// =========================
