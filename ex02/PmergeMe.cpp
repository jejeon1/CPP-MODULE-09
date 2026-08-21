#include "PmergeMe.hpp"

#include <stdexcept>
#include <cstdlib>
#include <climits>
#include <cctype>
#include <iostream>
#include <cerrno>
#include <utility>
#include <ctime>
#include <iomanip>

PmergeMe::PmergeMe()
    : _vector(), _deque(), _vectorTime(0.0), _dequeTime(0.0)
{
}

PmergeMe::PmergeMe(const PmergeMe& other)
    : _vector(other._vector), _deque(other._deque), _vectorTime(other._vectorTime), _dequeTime(other._dequeTime)
{
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
    if (this != &other)
    {
        _vector = other._vector;
        _deque = other._deque;
        _vectorTime = other._vectorTime;
        _dequeTime = other._dequeTime;
    }
    return *this;
}

PmergeMe::~PmergeMe()
{
}

void PmergeMe::parseInput(int ac, char **av)
{
    for (int i = 1; i < ac; ++i)
    {
        if (av[i][0] == '\0')
            throw std::runtime_error("Error");

        for (int j = 0; av[i][j] != '\0'; ++j)
        {
            if (!std::isdigit(static_cast<unsigned char>(av[i][j])))
                throw std::runtime_error("Error");
        }

        errno = 0;
        long value = std::strtol(av[i], NULL, 10);

        if (errno == ERANGE || value <= 0 || value > INT_MAX)
            throw std::runtime_error("Error");

        std::clock_t start = std::clock();

        _vector.push_back(static_cast<int>(value));

        std::clock_t end = std::clock();

        _vectorTime += static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000.0;

        start = std::clock();

        _deque.push_back(static_cast<int>(value));

        end = std::clock();

        _dequeTime += static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000.0;
    }
}

void PmergeMe::printBefore() const
{
    std::cout << "Before: ";

    for (std::vector<int>::const_iterator it = _vector.begin();
         it != _vector.end();
         ++it)
    {
        if (it != _vector.begin())
            std::cout << " ";

        std::cout << *it;
    }
    std::cout << std::endl;
}

void PmergeMe::printAfter() const
{
    std::cout << "After: ";

    for (std::vector<int>::const_iterator it = _vector.begin();
         it != _vector.end();
         ++it)
    {
        if (it != _vector.begin())
            std::cout << " ";

        std::cout << *it;
    }
    std::cout << std::endl;
}

void PmergeMe::printTimes() const
{
    std::cout << std::fixed << std::setprecision(5);

    std::cout << "Time to process a range of "
              << _vector.size()
              << " elements with std::vector : "
              << _vectorTime << " us"
              << std::endl;

    std::cout << "Time to process a range of "
              << _deque.size()
              << " elements with std::deque : "
              << _dequeTime << " us"
              << std::endl;
}

void PmergeMe::sortVector()
{
    std::clock_t start = std::clock();
    fordJohnsonVector(_vector);
    std::clock_t end = std::clock();
    _vectorTime += static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000.0;
}

void PmergeMe::sortDeque()
{
    std::clock_t start = std::clock();
    fordJohnsonDeque(_deque);
    std::clock_t end = std::clock();
    _dequeTime += static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000.0;
}

std::vector<std::pair<int, int> > PmergeMe::orderedPairsVector(
    const std::vector<std::pair<int, int> >& pairs,
    const std::vector<int>& winners) const
{
    std::vector<std::pair<int, int> > result;
    std::vector<bool> used(pairs.size(), false);

    for (std::size_t i = 0; i < winners.size(); ++i)
    {
        for (std::size_t j = 0; j < pairs.size(); ++j)
        {
            if (!used[j] && pairs[j].second == winners[i])
            {
                result.push_back(pairs[j]);
                used[j] = true;
                break;
            }
        }
    }
    return result;
}

std::deque<std::pair<int, int> > PmergeMe::orderedPairsDeque(
    const std::deque<std::pair<int, int> >& pairs,
    const std::deque<int>& winners) const
{
    std::deque<std::pair<int, int> > result;
    std::deque<bool> used(pairs.size(), false);

    for (std::size_t i = 0; i < winners.size(); ++i)
    {
        for (std::size_t j = 0; j < pairs.size(); ++j)
        {
            if (!used[j] && pairs[j].second == winners[i])
            {
                result.push_back(pairs[j]);
                used[j] = true;
                break;
            }
        }
    }
    return result;
}

std::vector<std::size_t>
PmergeMe::makeInsertionOrder(std::size_t bCount) const
{
    std::vector<std::size_t> order;

    if (bCount <= 1)
        return order;

    std::size_t jacobPrev = 1;
    std::size_t jacobCurrent = 3;
    std::size_t lastAdded = 1;

    while (lastAdded < bCount)
    {
        std::size_t groupEnd = jacobCurrent;

        if (groupEnd > bCount)
            groupEnd = bCount;

        for (std::size_t i = groupEnd; i > lastAdded; --i)
            order.push_back(i);

        lastAdded = groupEnd;

        std::size_t jacobNext =
            jacobCurrent + 2 * jacobPrev;

        jacobPrev = jacobCurrent;
        jacobCurrent = jacobNext;
    }
    return order;
}

void PmergeMe::fordJohnsonVector(std::vector<int>& values)
{
    if (values.size() <= 1)
        return;

    std::vector<std::pair<int, int> > pairs;

    for (std::size_t i = 0; i + 1 < values.size(); i += 2)
    {
        if (values[i] <= values[i + 1])
        {
            pairs.push_back(
                std::make_pair(values[i], values[i + 1]));
        }
        else
        {
            pairs.push_back(
                std::make_pair(values[i + 1], values[i]));
        }
    }

    bool hasUnpaired = (values.size() % 2 != 0);
    int unpairedValue = 0;

    if (hasUnpaired)
        unpairedValue = values.back();

    std::vector<int> winners;

    for (std::size_t i = 0; i < pairs.size(); ++i)
        winners.push_back(pairs[i].second);

    fordJohnsonVector(winners);

    std::vector<std::pair<int, int> > sortedPairs =
        orderedPairsVector(pairs, winners);

    std::vector<Element> mainChain;

    Element element;

    element.value = sortedPairs[0].first;
    element.pairIndex = 0;
    element.isA = false;
    element.hasPartner = true;

    mainChain.push_back(element);

    for (std::size_t i = 0; i < sortedPairs.size(); ++i)
    {
        Element element;

        element.value = sortedPairs[i].second;
        element.pairIndex = i;
        element.isA = true;
        element.hasPartner = true;

        mainChain.push_back(element);
    }

    std::size_t bCount = sortedPairs.size();

    if (hasUnpaired)
        ++bCount;

    std::vector<std::size_t> insertionOrder =
        makeInsertionOrder(bCount);

    for (std::size_t i = 0; i < insertionOrder.size(); ++i)
    {
        Element element;
        std::size_t bIndex = insertionOrder[i];

        element.isA = false;

        if (bIndex > sortedPairs.size())
        {
            element.value =  unpairedValue;
            element.pairIndex = sortedPairs.size();
            element.hasPartner = false; 
        }
        else
        {
            element.value = sortedPairs[bIndex - 1].first;
            element.pairIndex = bIndex - 1;
            element.hasPartner = true;
        }
        std::size_t partnerPos = mainChain.size();

        if (element.hasPartner)
        {
            for (std::size_t j = 0; j < mainChain.size(); ++j)
            {
                if (mainChain[j].isA == true && mainChain[j].pairIndex == element.pairIndex)
                {
                    partnerPos = j;
                    break;
                }
            }
        }
        
        std::size_t searchEnd;

        if (element.hasPartner)
            searchEnd = partnerPos;
        else
            searchEnd = mainChain.size();

        std::size_t left = 0;
        std::size_t right = searchEnd;

        while (left < right)
        {
            std::size_t mid = left + (right - left) / 2;

            if (mainChain[mid].value < element.value)
                left = mid + 1;
            else
                right = mid;
        }
        mainChain.insert(mainChain.begin() + left, element);
    }
    values.clear();
    for (std::size_t i = 0; i < mainChain.size(); ++i)
    {
        values.push_back(mainChain[i].value);
    }
}

void PmergeMe::fordJohnsonDeque(std::deque<int>& values)
{
    if (values.size() <= 1)
        return;

    std::deque<std::pair<int, int> > pairs;

    for (std::size_t i = 0; i + 1 < values.size(); i += 2)
    {
        if (values[i] <= values[i + 1])
        {
            pairs.push_back(
                std::make_pair(values[i], values[i + 1]));
        }
        else
        {
            pairs.push_back(
                std::make_pair(values[i + 1], values[i]));
        }
    }

    bool hasUnpaired = (values.size() % 2 != 0);
    int unpairedValue = 0;

    if (hasUnpaired)
        unpairedValue = values.back();

    std::deque<int> winners;

    for (std::size_t i = 0; i < pairs.size(); ++i)
        winners.push_back(pairs[i].second);

    fordJohnsonDeque(winners);

    std::deque<std::pair<int, int> > sortedPairs =
        orderedPairsDeque(pairs, winners);

    std::deque<Element> mainChain;

    Element element;

    element.value = sortedPairs[0].first;
    element.pairIndex = 0;
    element.isA = false;
    element.hasPartner = true;

    mainChain.push_back(element);

    for (std::size_t i = 0; i < sortedPairs.size(); ++i)
    {
        Element element;

        element.value = sortedPairs[i].second;
        element.pairIndex = i;
        element.isA = true;
        element.hasPartner = true;

        mainChain.push_back(element);
    }

    std::size_t bCount = sortedPairs.size();

    if (hasUnpaired)
        ++bCount;

    std::vector<std::size_t> insertionOrder =
        makeInsertionOrder(bCount);

    for (std::size_t i = 0; i < insertionOrder.size(); ++i)
    {
        Element element;
        std::size_t bIndex = insertionOrder[i];

        element.isA = false;

        if (bIndex > sortedPairs.size())
        {
            element.value =  unpairedValue;
            element.pairIndex = sortedPairs.size();
            element.hasPartner = false; 
        }
        else
        {
            element.value = sortedPairs[bIndex - 1].first;
            element.pairIndex = bIndex - 1;
            element.hasPartner = true;
        }
        std::size_t partnerPos = mainChain.size();

        if (element.hasPartner)
        {
            for (std::size_t j = 0; j < mainChain.size(); ++j)
            {
                if (mainChain[j].isA == true && mainChain[j].pairIndex == element.pairIndex)
                {
                    partnerPos = j;
                    break;
                }
            }
        }
        
        std::size_t searchEnd;

        if (element.hasPartner)
            searchEnd = partnerPos;
        else
            searchEnd = mainChain.size();

        std::size_t left = 0;
        std::size_t right = searchEnd;

        while (left < right)
        {
            std::size_t mid = left + (right - left) / 2;

            if (mainChain[mid].value < element.value)
                left = mid + 1;
            else
                right = mid;
        }
        mainChain.insert(mainChain.begin() + left, element);
    }
    values.clear();
    for (std::size_t i = 0; i < mainChain.size(); ++i)
    {
        values.push_back(mainChain[i].value);
    }
}