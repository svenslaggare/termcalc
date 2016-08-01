#pragma once
#include <vector>
#include <cstdint>
#include <string>

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
	DecimalPoint,
	ImaginaryUnit
};

//Represents a numeric constant
class NumericConstant {
private:
	std::vector<NumericConstantChar> mChars;
public:
	NumericConstant();

	//Creates a new numeric constant from the given string
	NumericConstant(std::string str);

	//Creates a new numeric constant from the given 64-bits integer
	NumericConstant(std::int64_t value);

	//Creates a new numeric constant from the given 64-bits float
	NumericConstant(double value);

	//Returns the character in the constant
	const std::vector<NumericConstantChar>& chars() const;

	bool operator==(const NumericConstant& rhs) const;
	bool operator!=(const NumericConstant& rhs) const;
};