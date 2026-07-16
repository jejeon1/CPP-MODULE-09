#include "RPN.hpp"
#include <stdexcept>
#include <sstream>
#include <cctype>

RPN::RPN() : _s()
{
}

RPN::RPN(const RPN& other) : _s(other._s)
{
}

RPN& RPN::operator=(const RPN& other)
{
    if (this != &other)
        this->_s = other._s;
    return *this;
}

RPN::~RPN()
{
}

bool RPN::isOperator(const std::string& token) const
{
    if (token == "+" || token == "-" || token == "*" || token == "/")
        return true;
    return false;
}

bool RPN::isNumber(const std::string& token) const
{
    if (token.length() == 1)
    {
        if (isdigit(static_cast<unsigned char>(token[0])))
            return true;
    }
    return false;
}

int RPN::calculate(const std::string& expression)
{
    std::stringstream ss(expression);
    std::string token;

    while (ss >> token)
    {
        if (isNumber(token))
        {
            this->_s.push(token[0] - '0');
        }
        else if (isOperator(token))
        {
            if (this->_s.size() < 2)
                throw std::runtime_error("Error"); 
            int right = this->_s.top();
            this->_s.pop();
            int left = this->_s.top();
            this->_s.pop();
            if (token == "+")
                this->_s.push(left + right);
            else if (token == "-")
                this->_s.push(left - right);
            else if (token == "*")
                this->_s.push(left * right);
            else if (token == "/")
            {
                if (right == 0)
                    throw std::runtime_error("Error");
                else
                    this->_s.push(left / right);
            }
        }
        else
            throw std::runtime_error("Error");
    }
    if (this->_s.size() != 1)
        throw std::runtime_error("Error");
    return this->_s.top();
}
