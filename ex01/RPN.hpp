#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN
{
    private:
        std::stack<int> _s;
    public:
        RPN();
        RPN(const RPN& other);
        RPN& operator=(const RPN& other);
        ~RPN();
        bool isOperator(const std::string& str) const;
        bool isNumber(const std::string& str) const;
        int calculate(const std::string& expression);
};

#endif
