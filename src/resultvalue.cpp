#include "resultvalue.h"

ResultValue::ResultValue(std::int64_t value)
	: mType(ResultValueType::INTEGER), mIntValue(value), mFloatValue(value) {

}

ResultValue::ResultValue(double value)
	:  mType(ResultValueType::FLOAT), mIntValue(value), mFloatValue(value) {

}

ResultValue::ResultValue(ResultValueType type, double value)
	:  mType(type), mIntValue(value), mFloatValue(value) {

}

ResultValue::ResultValue()
	:  mType(ResultValueType::FLOAT), mIntValue(0), mFloatValue(0.0) {

}

ResultValue::ResultValue(ResultValueType type, std::int64_t intValue, double floatValue)
	:  mType(type), mIntValue(intValue), mFloatValue(floatValue) {

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

bool ResultValue::operator==(const ResultValue& rhs) const {
	if (mType == rhs.mType) {
		if (mType == ResultValueType::FLOAT) {
			return mFloatValue == rhs.mFloatValue;
		} else {
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
		if (mType == ResultValueType::FLOAT) {
			return mFloatValue <= rhs.mFloatValue;
		} else {
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
	if (value.type() == ResultValueType::FLOAT) {
		os << value.floatValue();
	} else {
		os << value.intValue();
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
	}
	return os;
}