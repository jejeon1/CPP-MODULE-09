#include "PmergeMe.hpp"
#include <iostream>
#include <stdexcept>

int main(int ac, char **av)
{
    try
    {
        if (ac < 2)
            throw std::runtime_error("Error");

        PmergeMe sorter;

        sorter.parseInput(ac, av);

        sorter.printBefore();

        sorter.sortVector();
        sorter.sortDeque();

        sorter.printAfter();
        sorter.printTimes();

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}