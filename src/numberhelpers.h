#pragma once
#include <string>

//Contains helper methods for numbers
namespace NumberHelpers {
	//Converts the given value into a string with the given base
	std::string toBase(std::int64_t value, int base, std::string prefix = "");

	//Calculates the negated binary string
	std::string getBinaryNegated(std::string binStr, int size);

	//Converts the given value into a string with the binary base
	std::string toBaseBinary(std::int64_t value);
}