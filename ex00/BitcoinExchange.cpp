#include "BitcoinExchange.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cctype>

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _database(other._database)
{
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
		this->_database = other._database;
	return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

bool BitcoinExchange::badInput(const std::string& line) const
{
	std::cout << "Error: bad input => " << line << std::endl;
	return false;
}

bool BitcoinExchange::isLeapYear(int year) const
{
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

bool BitcoinExchange::isValidDate(const std::string& date, const std::string& line) const
{
	if (date.length() != 10)
		return badInput(line);
	for (int i = 0; i < static_cast<int>(date.length()); i++)
	{
		if (i == 4 || i == 7)
		{
			if (date[i] != '-')
				return badInput(line);
		}
		else if (!isdigit(date[i]))
			return badInput(line);
	}
	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());

	if (month < 1 || month > 12 || day < 1 || day > 31)
		return badInput(line);
	if (month == 4 || month == 6 || month == 9 || month == 11)
	{
		if (day > 30)
			return badInput(line);
	}
	if (month == 2)
	{
		if (isLeapYear(year))
		{
			if (day > 29)
				return badInput(line);
		}
		else
		{
			if (day > 28)
				return badInput(line);
		}
	}
	return true;
}

bool BitcoinExchange::isValidNumber(const std::string& str) const
{
	if (str.empty())
		return false;

	bool hasDot = false;

	for (size_t i = 0; i < str.length(); i++)
	{
		if (isdigit(str[i]))
			continue;

		if (str[i] == '.')
		{
			if (hasDot)
				return false;
			hasDot = true;
		}
		else
			return false;
	}
	if (!isdigit(str[0]) || !isdigit(str[str.length() - 1]))
		return false;

	return true;
}

bool BitcoinExchange::isValidValue(const std::string& value, const std::string& line) const
{
	if (value.empty())
		return badInput(line);
	if (value[0] == '-')
	{
		std::cout << "Error: not a positive number." << std::endl;
		return false;
	}
	if (!isValidNumber(value))
		return badInput(line);

	double num = std::atof(value.c_str());

	if (num > 1000)
	{
		std::cout << "Error: too large a number." << std::endl;
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
	if (line != "date,exchange_rate")
    	throw std::runtime_error("Error: bad database header");
	while (std::getline(file, line))
	{
		if (line.empty())
			continue;
		std::stringstream ss(line);
		std::string date;
		std::string rateStr;

		if (!std::getline(ss, date, ',') || !std::getline(ss, rateStr))
			throw std::runtime_error("Error: bad database format");
		if (!isValidNumber(rateStr))
			throw std::runtime_error("Error: bad rate");
		if (date.length() != 10)
			throw std::runtime_error("Error: bad date");
		_database[date] = std::atof(rateStr.c_str());
	}
}

void BitcoinExchange::processInputFile(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open file.");

	std::string line;
	const std::string firstLine = "date | value";

	if (!std::getline(file, line))
		throw std::runtime_error("Error: empty input");

	if (line != firstLine)
		throw std::runtime_error("Error: bad input");

	while (std::getline(file, line))
	{
		std::string date;
		std::string value;

		if (line.empty())
			continue;

		size_t pos = line.find('|');

		if (pos == std::string::npos || line.find('|', pos + 1) != std::string::npos)
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}
		if (pos == 0 || pos + 1 >= line.length())
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}
		if (line[pos - 1] != ' ' || line[pos + 1] != ' ')
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}
		date = line.substr(0, pos - 1);
		value = line.substr(pos + 2);

		if (!isValidDate(date, line))
			continue;
		if (!isValidValue(value, line))
			continue;
		double amount = std::atof(value.c_str());
		std::map<std::string, double>::iterator it = _database.lower_bound(date);
		if (it == _database.end())
		{
			--it;	
		}
		else if (it->first != date)
		{
			if (it == _database.begin())
			{
				std::cout << "Error: bad input => " << line << std::endl;
				continue;
			}
			--it;
		}
		std::cout << date << " => " << amount << " = " << amount * it->second << std::endl;
	}
}