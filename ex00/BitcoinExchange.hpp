#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>


class BitcoinExchange
{
private:
	std::map<std::string, double> _database;

	bool isValidRate(const std::string& str) const;
	bool isValidDate(const std::string& str) const;
	bool isLeapYear(int year) const;
public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange& other);
	BitcoinExchange& operator=(const BitcoinExchange& other);
	~BitcoinExchange();

	void loadDatabase();
	void processInputFile(const std::string& filename);
};

#endif