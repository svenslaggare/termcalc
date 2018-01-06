#include "numberhelpers.h"

namespace {
	const std::string alphaNumeric = "0123456789abcdefghijklmnopqrstuvwxyz";

	//Calculates the negated binary string
	std::string getBinaryNegated(std::string binStr, int size) {
		//Invert all bits
		std::string negBin = binStr;
		for (auto i = 0; i < size; i++) {
			if (binStr[i] == '0') {
				negBin[i] = '1';
			} else {
				negBin[i] = '0';
			}
		}

		//Add one
		for (int i = size - 1; i >= 0; i--) {
			char bit = negBin[i];

			if (bit == '0') {
				negBin[i] = '1';
				break;
			} else if (bit == '1') {
				negBin[i] = '0';
			}
		}

		return negBin;
	}
}

//Converts the given value into a string with the given base
std::string NumberHelpers::toBase(std::int64_t value, int base, std::string prefix) {
	std::string res;
	bool isNegative = value < 0;
	value = std::abs(value);
	while (value > 0) {
		res += alphaNumeric[value % base];
		value /= base;
	}

	return (isNegative ? "-" : "") + prefix + std::string(res.rbegin(), res.rend());
}

//Converts the given value into a string with the binary base
std::string NumberHelpers::toBaseBinary(std::int64_t value, int size) {
	//Convert to string
	bool isNegative = value < 0;
	std::string binStr = toBase(std::abs(value), 2);

	if (isNegative) {
		//Pad with zeros
		auto padding = size - binStr.size();
		for (auto i = 0; i < padding; i++) {
			binStr.insert(binStr.begin(), '0');
		}

		return getBinaryNegated(binStr, size);
	} else {
		return binStr;
	}
}