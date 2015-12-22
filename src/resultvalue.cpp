#include "resultvalue.h"
#include <cmath>

namespace {
	static constexpr double EPSILON = 0.0000000001;
}

ResultValue::ResultValue()
	:  mType(ResultValueType::FLOAT),
	   mIntValue(0),
	   mFloatValue(0.0),
	   mComplexValue(0.0) {

}

ResultValue::ResultValue(ResultValueType type, std::int64_t intValue, double floatValue, Complex complexValue)
	:  mType(type),
	   mIntValue(intValue),
	   mFloatValue(floatValue),
	   mComplexValue(complexValue) {

}

ResultValue::ResultValue(std::int64_t value)
	: mType(ResultValueType::INTEGER),
	  mIntValue(value),
	  mFloatValue(value),
	  mComplexValue(value) {

}

ResultValue::ResultValue(double value)
	:  mType(ResultValueType::FLOAT),
	   mIntValue((std::int64_t)value),
	   mFloatValue(value),
	   mComplexValue(value) {

}

ResultValue::ResultValue(Complex value)
	:  mType(ResultValueType::COMPLEX),
	   mIntValue((std::int64_t)value.real()),
	   mFloatValue(value.real()),
	   mComplexValue(value) {

}

ResultValueType ResultValue::type() const {
	return mType;
}

std::int64_t ResultValue::intValue() const {
	return mIntValue;
}

double ResultValue::floatValue() const {
	return mFloatValue;
}

Complex ResultValue::complexValue() const {
	return mComplexValue;
}

ResultValue ResultValue::convertTo(ResultValueType type) const {
	return ResultValue(type, mIntValue, mFloatValue, mComplexValue);
}

std::string ResultValue::toString() {
	switch (mType) {
		case ResultValueType::FLOAT:
			return std::to_string(floatValue());
		case ResultValueType::INTEGER:
			return std::to_string(intValue());
		case ResultValueType::COMPLEX:
			if (mComplexValue.imag() <= EPSILON) {
				return std::to_string(mComplexValue.real());
			} else {
				return std::to_string(mComplexValue.real()) + "+" + std::to_string(mComplexValue.imag()) + "i";
			}
	}
}

bool ResultValue::operator==(const ResultValue& rhs) const {
	if (mType == rhs.mType) {
		switch (mType) {
			case ResultValueType::FLOAT:
				return std::abs(mFloatValue - rhs.mFloatValue) <= EPSILON;
			case ResultValueType::INTEGER:
				return mIntValue == rhs.mIntValue;
			case ResultValueType::COMPLEX:
				return std::abs(mComplexValue.real() - rhs.mComplexValue.real()) <= EPSILON
					&& std::abs(mComplexValue.imag() - rhs.mComplexValue.imag()) <= EPSILON;
		}
	} else {
		return false;
	}
}

bool ResultValue::operator!=(const ResultValue& rhs) const {
	return !(*this == rhs);
}

bool ResultValue::operator<=(const ResultValue& rhs) const {
	if (mType == rhs.mType) {
		switch (mType) {
			case ResultValueType::FLOAT:
				return mFloatValue <= rhs.mFloatValue;
			case ResultValueType::INTEGER:
				return mIntValue <= rhs.mIntValue;
			case ResultValueType::COMPLEX:
				return false;
		}
	} else {
		return false;
	}
}

double ResultValue::operator+(double value) const {
	return mFloatValue + value;
}

std::ostream& operator<<(std::ostream& os, ResultValue value) {
//	os << value.toString();

	switch (value.type()) {
		case ResultValueType::FLOAT:
			os << value.floatValue();
			break;
		case ResultValueType::INTEGER:
			os << value.intValue();
			break;
		case ResultValueType::COMPLEX:
			if (value.complexValue().imag() <= EPSILON) {
				os << value.complexValue().real();
			} else {
				os << value.complexValue().real() << "+" << value.complexValue().imag() << "i";
			}
			break;
	}

	return os;
}

std::ostream& operator<<(std::ostream& os, ResultValueType value) {
	switch (value) {
	case ResultValueType::INTEGER:
		os << "INTEGER";
		break;
	case ResultValueType::FLOAT:
		os << "FLOAT";
		break;
	case ResultValueType::COMPLEX:
		os << "COMPLEX";
		break;
	}
	return os;
}