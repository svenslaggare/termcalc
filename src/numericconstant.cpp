#include "numericconstant.h"
#include <stdexcept>

NumericConstant::NumericConstant()
	: mChars({ NumericConstantChar::Zero }) {

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
	: NumericConstant(std::to_string(value)){

}

const std::vector<NumericConstantChar>& NumericConstant::chars() const {
	return mChars;
}

std::string NumericConstant::toString() const {
	std::string str;
	str.reserve(mChars.size());

	for (auto c : mChars) {
		switch (c) {
			case NumericConstantChar::Zero:
				str += '0';
				break;
			case NumericConstantChar::One:
				str += '1';
				break;
			case NumericConstantChar::Two:
				str += '2';
				break;
			case NumericConstantChar::Three:
				str += '3';
				break;
			case NumericConstantChar::Four:
				str += '4';
				break;
			case NumericConstantChar::Five:
				str += '5';
				break;
			case NumericConstantChar::Six:
				str += '6';
				break;
			case NumericConstantChar::Seven:
				str += '7';
				break;
			case NumericConstantChar::Eight:
				str += '8';
				break;
			case NumericConstantChar::Nine:
				str += '9';
				break;
			case NumericConstantChar::MinusSign:
				str += '-';
				break;
			case NumericConstantChar::PlusSign:
				str += '+';
				break;
			case NumericConstantChar::DecimalPoint:
				str += '.';
				break;
			case NumericConstantChar::ImaginaryUnit:
				str += 'i';
				break;
		}
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
