#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include "../termcalc.h"

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
	A,
	B,
	C,
	D,
	E,
	F,
	MinusSign,
	PlusSign,
	DecimalPoint,
	ImaginaryUnit,
};

namespace NumericConstantChars {
	//Returns the character for the given numeric character
	char getChar(NumericConstantChar numChar);

	//Returns the numeric character for the given character. If invalid, zero is returned
	NumericConstantChar getChar(char c);

	const NumericConstantChar Zero = NumericConstantChar::Zero;
	const NumericConstantChar One = NumericConstantChar::One;
	const NumericConstantChar Two = NumericConstantChar::Two;
	const NumericConstantChar Three = NumericConstantChar::Three;
	const NumericConstantChar Four = NumericConstantChar::Four;
	const NumericConstantChar Five = NumericConstantChar::Five;
	const NumericConstantChar Six = NumericConstantChar::Six;
	const NumericConstantChar Seven = NumericConstantChar::Seven;
	const NumericConstantChar Eight = NumericConstantChar::Eight;
	const NumericConstantChar Nine = NumericConstantChar::Nine;
	const NumericConstantChar A = NumericConstantChar::A;
	const NumericConstantChar B = NumericConstantChar::B;
	const NumericConstantChar C = NumericConstantChar::C;
	const NumericConstantChar D = NumericConstantChar::D;
	const NumericConstantChar E = NumericConstantChar::E;
	const NumericConstantChar F = NumericConstantChar::F;
}

//A vector of numeric characters
using NumericChars = std::vector<NumericConstantChar>;

//Represents a numeric constant
class NumericConstant {
private:
	NumericChars mChars;
	unsigned char mBase = 10;
public:
	NumericConstant();

	//Creates a numeric constant from the given characters. No error checking is made.
	NumericConstant(const std::initializer_list<NumericConstantChar>& chars);

	//Creates a numeric constant from the given characters. No error checking is made.
	NumericConstant(const std::vector<NumericConstantChar>& chars);

	//Creates a new numeric constant from the given string
	NumericConstant(const std::string& str);

	//Creates a new numeric constant from the given 64-bits integer
	NumericConstant(std::int64_t value);

	//Creates a new numeric constant from the given 64-bits float
	NumericConstant(double value);

	//Creates a new numeric constant from the given 64-bits complex
	NumericConstant(Complex value);

	//Returns the character in the constant
	const NumericChars& chars() const;

	//Returns the base
	unsigned char base() const;

	//Sets the base
	void base(unsigned char newBase);

	//Appends the given character to the current constant
	NumericConstant& append(NumericConstantChar numChar);

	//Returns a string representation
	std::string toString() const;

	bool operator==(const NumericConstant& rhs) const;
	bool operator!=(const NumericConstant& rhs) const;
};