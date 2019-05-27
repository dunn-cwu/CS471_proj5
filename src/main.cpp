#include <iostream>
#include "flowshopblocking.h"

int main()
{
    int seq[5] = {1, 2, 3, 4, 5};
    fshop::FlowshopBlocking fsBasic("DataFiles/0.txt");
    auto result = fsBasic.calcObjective(seq, 5);
    std::cout << "cmax: " << result->cmax << std::endl;
    std::cout << "tft: " << result->totalFlowTime << std::endl;
    return 0;
}