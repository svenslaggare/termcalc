#include "numericconstant.h"
#include <stdexcept>

NumericConstant::NumericConstant()
	: mChars({ NumericConstantChar::Zero }) {

}

NumericConstant::NumericConstant(std::string str) {
	bool hasMinusSign = false;
	bool hasDecimalPoint = false;
	bool hasImaginaryUnit = false;

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
				if (hasMinusSign) {
					throw std::runtime_error("The constant already has a minus sign.");
				}

				hasMinusSign = true;
				mChars.push_back(NumericConstantChar::MinusSign);
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
