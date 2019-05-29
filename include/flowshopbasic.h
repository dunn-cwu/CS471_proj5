/**
 * @file flowshopbasic.h
 * @author Andrew Dunn (Andrew.Dunn@cwu.edu)
 * @brief Contains the FlowshopBasic class which can solve a basic
 * flowshop scheduling problem for a given job sequence. The FlowshopBasic
 * class is also used as a base class for the FlowshopBlocking and FlowshopNoWait
 * classes.
 * @version 0.1
 * @date 2019-05-24
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef __FLOWSHOPBASIC_H
#define __FLOWSHOPBASIC_H

#include <stddef.h>
#include <stdexcept>
#include <memory>
#include <string>

namespace fshop
{
    /**
     * @brief The FlowshopSolution struct houses all solution data returned
     * from calculating the objective flowshop function. This includes the
     * cmax value, total flow time, job sequence used, start time matrix,
     * and departure time matrix.
     */
    struct FlowshopSolution
    {
        FlowshopSolution(int** _startimeMatrix, int** _departTimeMatrix, size_t _tMatrixRows, int* _jobSeq, size_t _seqSize, int _cmax, int _totalFlowTime);
        ~FlowshopSolution();

        const size_t seqSize; /** Number of jobs in job sequence */
        const size_t numMachines; /** Number of machines executing jobs */
        const int cmax; /** Flowshop cmax value, which is the departure time of the last job */
        const int totalFlowTime; /** Flowshop total flow time value, which is the sum of all departure times on the last machine */

        const int* const getJobSeq();
        std::string getJobSeqAsString();
        const int** const getStartTimeMatrix();
        const int** const getDepartTimeMatrix();

        bool outputTimesCsv(const std::string& fileNamePrefix);

        // Copy constructor
        FlowshopSolution(const FlowshopSolution& obj);

        // Move constructor
        FlowshopSolution(FlowshopSolution&& obj);

        // Delete copy assignment
        FlowshopSolution& operator=(const FlowshopSolution& obj) = delete;

        // Delete move assignment
        FlowshopSolution& operator=(FlowshopSolution&& obj) = delete;
    private:
        int* jobSequence; /** Pointer to the job sequence array */
        int** startTimeMatrix; /** Pointer to the start times matrix */
        int** departTimeMatrix; /** Pointer to the departure times matrix */
    };

    /**
     * @brief The FlowshopBasic class runs the standard flowshop scheduling problem
     * for a given job-machine processing time matrix that is read from a file. The run()
     * method takes the specific job sequence being calculated. This class also serves as
     * a base class for the Flowshop with Blocking and Flowshop with No Wait problem variants.
     */
    class FlowshopBasic
    {
    public:
        FlowshopBasic(const char* procTimeMatrixFile);
        virtual ~FlowshopBasic();
        virtual std::unique_ptr<FlowshopSolution> calcObjective(int* seq, size_t seqSize);

        virtual int getProcessingTime(size_t machine, size_t job);
        virtual size_t getTotalJobs();
        virtual size_t getTotalMachines();
        virtual size_t getFuncCallCounts();

        // Delete copy/move constructors and assignments
        FlowshopBasic(const FlowshopBasic& o) = delete;
        FlowshopBasic(const FlowshopBasic&& o) = delete;
        FlowshopBasic& operator=(const FlowshopBasic& o) = delete;
        FlowshopBasic& operator=(const FlowshopBasic&& o) = delete;
    protected:
        int** procTimeMatrix;  /** The job processing time matrix, which is read from a file */
        int** startTimeMatrix; /** The job start times matrix */
        size_t ptMatrixRows; /** The number of rows (machines) in the processing time matrix */
        size_t ptMatrixCols; /** The number of columns (jobs) in the processing time matrix */
        size_t funcCallCounter; /** Keeps track of the number of times run() is called */

        virtual void validateParams(int* seq, size_t seqSize);
        virtual int** allocTimeMatrix(size_t rows, size_t cols);
        virtual void initTimeMatrix(int** compTimeMatrix, int* seq, size_t rows, size_t cols);
        virtual void calcTimeMatrix(int** compTimeMatrix, int* seq, size_t rows, size_t cols);
        virtual void calcStartTimeCol(int** startTimeMatrix, int** departTimeMatrix, int* seq, size_t curCol, size_t rows, size_t cols);
        virtual int getCmax(int** compTimeMatrix, size_t rows, size_t cols);
        virtual int getTFT(int** compTimeMatrix, size_t rows, size_t cols);
    };
}

#endif

// =========================
// End of flowshopbasic.h
// =========================
