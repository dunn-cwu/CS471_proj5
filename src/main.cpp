#include <iostream>
#include "neh.h"
#include "flowshopblocking.h"
#include "flowshopnowait.h"
#include "mem.h"

int main()
{
    int seq[5] = {1, 2, 3, 4, 5};
    fshop::FlowshopBlocking fsBasic("DataFiles/1.txt");
    auto result = fsBasic.calcObjective(seq, 5);
    std::cout << "cmax: " << result->cmax << std::endl;
    std::cout << "tft: " << result->totalFlowTime << std::endl;

    std::cout << "seq: ";

    for (size_t i = 0; i < result->seqSize; i++)
    {
        std::cout << result->getJobSeq()[i] << ", ";
    }

    std::cout << std::endl;

    std::cout << "Start times:" << std::endl;
    util::outputMatrix(std::cout, result->getStartTimeMatrix(), result->numMachines, result->seqSize);

    std::cout << std::endl << "Depart times:" << std::endl;
    util::outputMatrix(std::cout, result->getDepartTimeMatrix(), result->numMachines, result->seqSize);

    auto bestResult = fshop::NEH::run(&fsBasic);

    std::cout << "cmax: " << bestResult->cmax << std::endl;
    std::cout << "tft: " << bestResult->totalFlowTime << std::endl;

    std::cout << "seq: ";

    for (size_t i = 0; i < bestResult->seqSize; i++)
    {
        std::cout << bestResult->getJobSeq()[i] << ", ";
    }

    std::cout << std::endl;

    std::cout << "Start times:" << std::endl;
    util::outputMatrix(std::cout, bestResult->getStartTimeMatrix(), bestResult->numMachines, bestResult->seqSize);

    std::cout << std::endl << "Depart times:" << std::endl;
    util::outputMatrix(std::cout, bestResult->getDepartTimeMatrix(), bestResult->numMachines, bestResult->seqSize);

    return 0;
}