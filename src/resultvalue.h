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
	long mLongValue;
	double mDoubleValue;

	ResultValue(ResultValueType type, long longValue, double doubleValue);
public:	
	//Creates a new long value
	ResultValue(long value);

	//Creates a new double value
	ResultValue(double value);

	//Creates a new value of the given type.
	ResultValue(ResultValueType type, double value);

	//Crate a new result value
	ResultValue();

	//Returns the type of the value
	ResultValueType type() const;

	//Returns the long value
	long longValue() const;

	//Returns the double value
	double doubleValue() const;

	//Converts the current value to the given type
	ResultValue convertTo(ResultValueType type) const;

	//Compare operators
	bool operator==(const ResultValue& rhs) const;
	bool operator!=(const ResultValue& rhs) const;
	bool operator<=(const ResultValue& rhs) const;

	double operator+(double value) const;
};

std::ostream& operator<<(std::ostream& os, ResultValue value);
std::ostream& operator<<(std::ostream& os, ResultValueType value);