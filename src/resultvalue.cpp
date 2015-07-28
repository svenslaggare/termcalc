#include "resultvalue.h"

ResultValue::ResultValue(long value)
	: mType(ResultValueType::INTEGER), mLongValue(value), mDoubleValue(value) {

}

ResultValue::ResultValue(double value)
	:  mType(ResultValueType::FLOAT), mLongValue(value), mDoubleValue(value) {

}

ResultValue::ResultValue(ResultValueType type, double value)
	:  mType(type), mLongValue(value), mDoubleValue(value) {

}

ResultValue::ResultValue()
	:  mType(ResultValueType::FLOAT), mLongValue(0), mDoubleValue(0.0) {

}


ResultValueType ResultValue::type() const {
	return mType;
}


long ResultValue::longValue() const {
	return mLongValue;
}


double ResultValue::doubleValue() const {
	return mDoubleValue;
}

bool ResultValue::operator==(const ResultValue& rhs) const {
	if (mType == rhs.mType) {
		if (mType == ResultValueType::FLOAT) {
			return mDoubleValue == rhs.mDoubleValue;
		} else {
			return mLongValue == rhs.mLongValue;
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
			return mDoubleValue <= rhs.mDoubleValue;
		} else {
			return mLongValue <= rhs.mLongValue;
		}
	} else {
		return false;
	}
}

double ResultValue::operator+(double value) const {
	return mDoubleValue + value;
}

std::ostream& operator<<(std::ostream& os, ResultValue value) {
	if (value.type() == ResultValueType::FLOAT) {
		os << value.doubleValue();
	} else {
		os << value.longValue();
	}

	return os;
}