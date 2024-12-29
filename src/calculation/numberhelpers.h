#pragma once
#include <string>
#include <cstdint>

//Contains helper methods for numbers
namespace NumberHelpers {
	//Converts the given value into a string with the given base
	std::string toBase(std::int64_t value, int base, std::string prefix = "");

	//Converts the given value into a string with the binary base of the given size
	std::string toBaseBinary(std::int64_t value, int size = 64);
}
