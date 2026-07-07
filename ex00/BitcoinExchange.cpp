#include "BitcoinExchange.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cctype>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _database(other._database)
{

}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
	{
		this->_database = other._database;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}

bool BitcoinExchange::isLeapYear(int year) const
{
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		return true;
	return false;
}

bool BitcoinExchange::isValidDate(const std::string& str) const
{
	if (str.length() != 10)
		return false;
	for (int i = 0; i < str.length(); i++)
	{
		if (i == 4 || i == 7)
		{
			if (str[i] != '-')
				return false;
		}
		else if (!isdigit(str[i]))
			return false;
	}
	int year = std::atoi(str.substr(0, 4).c_str());
	int month = std::atoi(str.substr(5, 2).c_str());
	int day = std::atoi(str.substr(8, 2).c_str());

	if (month < 1 || month > 12)
		return false;
	if (day < 1 || day > 31)
		return false;
	if (month == 4 || month == 6 || month == 9 || month == 11)
	{
		if (day > 30)
			return false;
	}
	if (month == 2)
	{
		if (isLeapYear(year))
		{
			if (day > 29)
				return false;
		}
		else
		{
			if (day > 28)
				return false;
		}
	}
	return true;
}

bool BitcoinExchange::isValidRate(const std::string& str) const
{
	bool hasDot = false;

	if (str.empty())
		return false;
	if (!isdigit(str[0]) || !isdigit(str[str.length() - 1]))
		return false;
	for (int i = 0; str[i]; i++)
	{
		if (isdigit(str[i]))
			continue;
		else if (str[i] == '.')
		{
			if (hasDot == true)
				return false;
			hasDot = true;
		}
		else
			return false;
	}
	return true;
}

void BitcoinExchange::loadDatabase()
{
	std::ifstream file("data.csv");
	if (!file.is_open())
		throw std::runtime_error("Error: could not open data.csv");
	std::string line;

	if (!std::getline(file, line))
		throw std::runtime_error("Error: empty database");

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		std::stringstream ss(line);
		std::string date;
		std::string rateStr;

		if (!std::getline(ss, date, ',') || !std::getline(ss, rateStr))
			throw std::runtime_error("Error: bad database format");
		if (!isValidRate(rateStr))
			throw std::runtime_error("Error: rate");
		if (!isValidDate(date))
			throw std::runtime_error("Error: date");
		this->_database[date] = std::atof(rateStr.c_str());
	}
}

void BitcoinExchange::processInputFile(const std::string& filename)
{
	(void)filename;
}