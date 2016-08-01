#include "numericconstant.h"
#include <stdexcept>
#include <limits>
#include <sstream>

namespace {
	std::string toStringFull(double value) {
		std::stringstream stringstream;
		stringstream.precision(std::numeric_limits<double>::max_digits10);
		stringstream << value;
		return stringstream.str();
	}

	std::string toStringFull(Complex value) {
		std::string str = "";

		if (value.imag() == 0.0) {
			str = toStringFull(value.real());
		} else if (value.real() == 0.0) {
			str = toStringFull(value.imag()) + "i";
		} else {
			str += toStringFull(value.real());
			if (value.imag() > 0) {
				str += "+";
			}
			str += toStringFull(value.imag()) + "i";
		}

		return str;
	}
}
char NumericConstantChars::getChar(NumericConstantChar numChar) {
	switch (numChar) {
		case NumericConstantChar::Zero:
			return '0';
		case NumericConstantChar::One:
			return '1';
		case NumericConstantChar::Two:
			return '2';
		case NumericConstantChar::Three:
			return '3';
		case NumericConstantChar::Four:
			return '4';
		case NumericConstantChar::Five:
			return '5';
		case NumericConstantChar::Six:
			return '6';
		case NumericConstantChar::Seven:
			return'7';
		case NumericConstantChar::Eight:
			return '8';
		case NumericConstantChar::Nine:
			return '9';
		case NumericConstantChar::MinusSign:
			return '-';
		case NumericConstantChar::PlusSign:
			return'+';
		case NumericConstantChar::DecimalPoint:
			return '.';
		case NumericConstantChar::ImaginaryUnit:
			return 'i';
	}
}

NumericConstant::NumericConstant()
	: mChars({ NumericConstantChar::Zero }) {

}

NumericConstant::NumericConstant(const std::initializer_list<NumericConstantChar>& chars)
	: mChars(chars) {

}

NumericConstant::NumericConstant(std::string str) {
	bool hasMinusSign = false;
	bool hasPlusSign = false;
	bool hasDecimalPoint = false;
	bool hasImaginaryUnit = false;
	bool isComplexMode = false;

	std::size_t i = 0;
	for (auto c : str) {
		switch (c) {
			case '0':
				mChars.push_back(NumericConstantChar::Zero);
				break;
			case '1':
				mChars.push_back(NumericConstantChar::One);
				break;
			case '2':
				mChars.push_back(NumericConstantChar::Two);
				break;
			case '3':
				mChars.push_back(NumericConstantChar::Three);
				break;
			case '4':
				mChars.push_back(NumericConstantChar::Four);
				break;
			case '5':
				mChars.push_back(NumericConstantChar::Five);
				break;
			case '6':
				mChars.push_back(NumericConstantChar::Six);
				break;
			case '7':
				mChars.push_back(NumericConstantChar::Seven);
				break;
			case '8':
				mChars.push_back(NumericConstantChar::Eight);
				break;
			case '9':
				mChars.push_back(NumericConstantChar::Nine);
				break;
			case '-':
				if (i > 0 && !isComplexMode) {
					isComplexMode = true;
					hasMinusSign = false;
					hasPlusSign = false;
					hasDecimalPoint = false;
				}

				if (hasMinusSign) {
					throw std::runtime_error("The constant already has a minus sign.");
				}

				hasMinusSign = true;
				mChars.push_back(NumericConstantChar::MinusSign);
				break;
			case '+':
				if (i > 0 && !isComplexMode) {
					isComplexMode = true;
					hasMinusSign = false;
					hasPlusSign = false;
					hasDecimalPoint = false;
				}

				if (hasPlusSign) {
					throw std::runtime_error("The constant already has a plus sign.");
				}

				hasPlusSign = true;
				mChars.push_back(NumericConstantChar::PlusSign);
				break;
			case '.':
				if (hasDecimalPoint) {
					throw std::runtime_error("The constant already has a decimal point.");
				}

				hasDecimalPoint = true;
				mChars.push_back(NumericConstantChar::DecimalPoint);
				break;
			case 'i':
				if (hasImaginaryUnit) {
					throw std::runtime_error("The constant already has an imaginary unit.");
				}

				hasImaginaryUnit = true;
				mChars.push_back(NumericConstantChar::ImaginaryUnit);
				break;
			default:
				break;
		}

		i++;
	}
}

NumericConstant::NumericConstant(std::int64_t value)
	: NumericConstant(std::to_string(value)) {

}

NumericConstant::NumericConstant(double value)
	: NumericConstant(toStringFull(value)){

}

NumericConstant::NumericConstant(Complex value)
	: NumericConstant(toStringFull(value)) {

}

const std::vector<NumericConstantChar>& NumericConstant::chars() const {
	return mChars;
}

NumericConstant& NumericConstant::append(NumericConstantChar numChar) {
	mChars.push_back(numChar);
	return *this;
}

std::string NumericConstant::toString() const {
	std::string str;
	str.reserve(mChars.size());

	for (auto c : mChars) {
		str += NumericConstantChars::getChar(c);
	}

	return str;
}

bool NumericConstant::operator==(const NumericConstant& rhs) const {
	if (mChars.size() != rhs.mChars.size()) {
		return false;
	}

	for (std::size_t i = 0; i < mChars.size(); ++i) {
		if (mChars[i] != rhs.mChars[i]) {
			return false;
		}
	}

	return true;
}

bool NumericConstant::operator!=(const NumericConstant& rhs) const {
	return !(*this == rhs);
}
