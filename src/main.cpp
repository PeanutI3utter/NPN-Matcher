#include <boost/multiprecision/integer.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>

#include "BLIF/BooleanFunction.hpp"
#include "BLIF/Model.hpp"
#include "BLIF/Parser.hpp"
#include "Types/CompactBooleanFunction.hpp"
#include "Util/NPN.hpp"
#include "Util/Permutation.hpp"


enum class Option
{
	EXHAUSTIVE = 0,
	FLIPSWAP = 1
};


LLS1::Types::CompactBoolFunc readCompactBoolFuncFrom(const char* file)
{
	std::vector<LLS1::BLIF::Model> model =
		LLS1::BLIF::parseBLIF(file);
	LLS1::BLIF::BooleanFunction func =
		*dynamic_cast<LLS1::BLIF::BooleanFunction*>(&(*(model[0].modules[0])));

	return func;
}


int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cout << "Usage:" << std::endl
			<< "\tmain {file1} {file2} [option]" << std::endl << std::endl
			<< "\tOption:" << std::endl << "\t\t1: Exhaustive Search"
			<< std::endl << "\t\t2: FlipSwap Heuristic";
		return -1;
	}

	Option mode = Option::EXHAUSTIVE;
	if(argc == 4)
	{
		mode = static_cast<Option>(std::stoi(argv[3]));
	}


	switch (mode)
	{
		case Option::EXHAUSTIVE:
		{

			LLS1::Types::CompactBoolFunc a = readCompactBoolFuncFrom(argv[1]);
			LLS1::Types::CompactBoolFunc b = readCompactBoolFuncFrom(argv[2]);

			LLS1::Util::Exhaustive checker{a.num_inputs};
			bool result = checker.equal(a, b);

			if (result)
			{
				LLS1::Util::CompactManipulator& f = checker.func_manipulator; 
				std::cout << "NPN Equal" << std::endl;
			} else {
				std::cout << "Not NPN Equal" << std::endl;
			}
			std::cout << "Time Elapsed [µs]: " << checker.elapsed_microseconds << std::endl;
			std::cout << "Transformations checked: " << checker.transformations << std::endl;
			break;
		}
		case Option::FLIPSWAP:
		{
			LLS1::Types::CompactBoolFunc a = readCompactBoolFuncFrom(argv[1]);
			LLS1::Types::CompactBoolFunc b = readCompactBoolFuncFrom(argv[2]);

			LLS1::Util::FlipSwap checker{a.num_inputs};
			bool result = checker.equal(a, b);

			if (result)
			{
				std::cout << "NPN Equal" << std::endl;
			} else {
				std::cout << "Not NPN Equal" << std::endl;
			}
			std::cout << "Time Elapsed [µs]: " << checker.elapsed_microseconds << std::endl;
			std::cout << "Transformations checked: " << checker.transformations << std::endl;
			break;
		}
		default:
			throw std::runtime_error("Invalid Option selected.");
	}

	return 0;
}