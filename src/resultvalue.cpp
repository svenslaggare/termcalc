#include "resultvalue.h"
#include <cmath>

namespace {
	static constexpr double EPSILON = 0.0000000001;
}

ResultValue::ResultValue()
	:  mType(ResultValueType::NONE),
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
	std::stringstream strstream;
	strstream << *this;
	return strstream.str();
}

bool ResultValue::operator==(const ResultValue& rhs) const {
	if (mType == rhs.mType) {
		switch (mType) {
			case ResultValueType::NONE:
				return true;
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

double ResultValue::operator+(double value) const {
	return mFloatValue + value;
}

std::ostream& operator<<(std::ostream& os, ResultValue value) {
	switch (value.type()) {
		case ResultValueType::NONE:
			os << "NONE";
			break;
		case ResultValueType::FLOAT:
			os << value.floatValue();
			break;
		case ResultValueType::INTEGER:
			os << value.intValue();
			break;
		case ResultValueType::COMPLEX:
			if (std::abs(value.complexValue().imag()) <= EPSILON) {
				os << value.complexValue().real();
			} else {
				std::string sign = "+";
				if (value.complexValue().imag() < 0) {
					sign = "";
				}

				if (value.complexValue().real() != 0) {
					os << value.complexValue().real();
				} else {
					sign = "";
				}

				os << sign;

				if (std::abs(value.complexValue().imag()) != 1) {
					os << value.complexValue().imag();
				}

				if (value.complexValue().imag() == -1) {
					os << "-";
				}

				os << "i";
			}
			break;
	}

	return os;
}

std::ostream& operator<<(std::ostream& os, ResultValueType value) {
	switch (value) {
	case ResultValueType::NONE:
		os << "NONE";
		break;
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