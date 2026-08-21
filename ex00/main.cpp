#include "BitcoinExchange.hpp"
#include <iostream>
#include <stdexcept>

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}
	BitcoinExchange btc;

	try
	{
		btc.loadDatabase();
		btc.processInputFile(av[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}