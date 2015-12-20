#pragma once
#include <iostream>

//The type of the result value
enum class ResultValueType {
	FLOAT,
	INTEGER
};

class ResultValue {
private:
	ResultValueType mType;
	std::int64_t mIntValue;
	double mFloatValue;
public:
	//Crate a new result value
	ResultValue();

	//Create a new value of the given type
	ResultValue(ResultValueType type, std::int64_t intValue, double floatValue);

	//Creates a new int64 value
	ResultValue(std::int64_t value);

	//Creates a new double value
	ResultValue(double value);

	//Returns the type of the value
	ResultValueType type() const;

	//Returns the int value
	std::int64_t intValue() const;

	//Returns the float value
	double floatValue() const;

	//Converts the current value to the given type
	ResultValue convertTo(ResultValueType type) const;

	//Returns a string representation
	std::string toString();

	//Compare operators
	bool operator==(const ResultValue& rhs) const;
	bool operator!=(const ResultValue& rhs) const;
	bool operator<=(const ResultValue& rhs) const;

	double operator+(double value) const;
};

namespace std {
	template <>
	struct hash<ResultValueType> {
		std::size_t operator()(const ResultValueType& value) const {
			return std::hash<std::int64_t>()((std::int64_t)value);
		}
	};
}

std::ostream& operator<<(std::ostream& os, ResultValue value);
std::ostream& operator<<(std::ostream& os, ResultValueType value);