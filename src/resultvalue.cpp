#include "resultvalue.h"
#include <cmath>

ResultValue::ResultValue()
	:  mType(ResultValueType::FLOAT), mIntValue(0), mFloatValue(0.0) {

}

ResultValue::ResultValue(ResultValueType type, std::int64_t intValue, double floatValue)
	:  mType(type), mIntValue(intValue), mFloatValue(floatValue) {

}

ResultValue::ResultValue(std::int64_t value)
	: mType(ResultValueType::INTEGER), mIntValue(value), mFloatValue(value) {

}

ResultValue::ResultValue(double value)
	:  mType(ResultValueType::FLOAT), mIntValue((std::int64_t)value), mFloatValue(value) {

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

ResultValue ResultValue::convertTo(ResultValueType type) const {
	return ResultValue(type, mIntValue, mFloatValue);
}

std::string ResultValue::toString() {
	switch (mType) {
		case ResultValueType::FLOAT:
			return std::to_string(floatValue());
		case ResultValueType::INTEGER:
			return std::to_string(intValue());
	}
}

bool ResultValue::operator==(const ResultValue& rhs) const {
	if (mType == rhs.mType) {
		switch (mType) {
			case ResultValueType::FLOAT:
				return std::abs(mFloatValue - rhs.mFloatValue) <= 0.0000000001;
			case ResultValueType::INTEGER:
				return mIntValue == rhs.mIntValue;
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
		}
	} else {
		return false;
	}
}

double ResultValue::operator+(double value) const {
	return mFloatValue + value;
}

std::ostream& operator<<(std::ostream& os, ResultValue value) {
	os << value.toString();
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
	}
	return os;
}