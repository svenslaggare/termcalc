#include "numericconstant.h"
#include <stdexcept>
#include <limits>
#include <sstream>

namespace {
	std::string toStringFull(double value) {
		double fracPart;
		double intPart;
		fracPart = std::modf(value, &intPart);

		std::stringstream stringstream;
		stringstream.precision(std::numeric_limits<double>::max_digits10);
		stringstream << value;
		auto str = stringstream.str();

		if (fracPart == 0.0) {
			str += ".0";
		}

		return str;
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
			return '7';
		case NumericConstantChar::Eight:
			return '8';
		case NumericConstantChar::Nine:
			return '9';
		case NumericConstantChar::A:
			return 'a';
		case NumericConstantChar::B:
			return 'b';
		case NumericConstantChar::C:
			return 'c';
		case NumericConstantChar::D:
			return 'd';
		case NumericConstantChar::E:
			return 'e';
		case NumericConstantChar::F:
			return 'f';
		case NumericConstantChar::MinusSign:
			return '-';
		case NumericConstantChar::PlusSign:
			return '+';
		case NumericConstantChar::DecimalPoint:
			return '.';
		case NumericConstantChar::ImaginaryUnit:
			return 'i';
	}
}

NumericConstantChar NumericConstantChars::getChar(char c) {
	switch (c) {
		case '0':
			return NumericConstantChar::Zero;
		case '1':
			return NumericConstantChar::One;
		case '2':
			return NumericConstantChar::Two;
		case '3':
			return NumericConstantChar::Three;
		case '4':
			return NumericConstantChar::Four;
		case '5':
			return NumericConstantChar::Five;
		case '6':
			return NumericConstantChar::Six;
		case '7':
			return NumericConstantChar::Seven;
		case '8':
			return NumericConstantChar::Eight;
		case '9':
			return NumericConstantChar::Nine;
		case 'a':
			return NumericConstantChar::A;
		case 'b':
			return NumericConstantChar::B;
		case 'c':
			return NumericConstantChar::C;
		case 'd':
			return NumericConstantChar::D;
		case 'e':
			return NumericConstantChar::E;
		case 'f':
			return NumericConstantChar::F;
		case '-':
			return NumericConstantChar::MinusSign;
		case '+':
			return NumericConstantChar::PlusSign;
		case '.':
			return NumericConstantChar::DecimalPoint;
		case 'i':
			return NumericConstantChar::ImaginaryUnit;
		default:
			return NumericConstantChar::Zero;
	}
}

NumericConstant::NumericConstant()
	: mChars({ NumericConstantChar::Zero }) {

}

NumericConstant::NumericConstant(const std::initializer_list<NumericConstantChar>& chars)
	: mChars(chars) {

}

NumericConstant::NumericConstant(const std::vector<NumericConstantChar>& chars)
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
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				mChars.push_back(NumericConstantChars::getChar(c));
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
