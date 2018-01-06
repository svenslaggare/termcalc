#include "numbertype.h"
#include "../numberhelpers.h"
#include <cmath>
#include <complex>
#include <cctype>

NumericConstant NumberType::toNumericConstant(std::string str) const {
	std::size_t i = 0;
	char c = str[0];
	return parseNumber(str, c, i).numericValue();
}