#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <utility>
#include <cstddef>

struct Element
{
    int value;
    std::size_t pairIndex;
    bool isA;
    bool hasPartner;
};


class PmergeMe
{
    private:
        std::vector<int> _vector;
        std::deque<int> _deque;

        void fordJohnsonVector(std::vector<int>& values);

        std::vector<std::pair<int, int> > orderedPairs(
            const std::vector<std::pair<int, int> >& pairs,
            const std::vector<int>& winners) const;

        std::vector<std::size_t> makeInsertionOrder(
            std::size_t bCount) const;

    public:
        PmergeMe();
        PmergeMe(const PmergeMe& other);
        PmergeMe& operator=(const PmergeMe& other);
        ~PmergeMe();

        void parseInput(int ac, char **av);
        void printBefore() const;
        void printAfter() const;
        void printTimes() const;

        void sortVector();
        void sortDeque();
};

#endif