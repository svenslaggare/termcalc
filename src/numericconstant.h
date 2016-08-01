#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include "termcalc.h"

//Represents a character in a numeric constant
enum class NumericConstantChar : unsigned char {
	Zero,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	MinusSign,
	PlusSign,
	DecimalPoint,
	ImaginaryUnit
};

namespace NumericConstantChars {
	char getChar(NumericConstantChar numChar);
}

//Represents a numeric constant
class NumericConstant {
private:
	std::vector<NumericConstantChar> mChars;
public:
	NumericConstant();

	//Creates a numeric constant from the given characters. No error checking is made.
	NumericConstant(const std::initializer_list<NumericConstantChar>& chars);

	//Creates a new numeric constant from the given string
	NumericConstant(std::string str);

	//Creates a new numeric constant from the given 64-bits integer
	NumericConstant(std::int64_t value);

	//Creates a new numeric constant from the given 64-bits float
	NumericConstant(double value);

	//Creates a new numeric constant from the given 64-bits complex
	NumericConstant(Complex value);

	//Returns the character in the constant
	const std::vector<NumericConstantChar>& chars() const;

	//Appends the given character to the current constant
	NumericConstant& append(NumericConstantChar numChar);

	//Returns a string representation
	std::string toString() const;

	bool operator==(const NumericConstant& rhs) const;
	bool operator!=(const NumericConstant& rhs) const;
};