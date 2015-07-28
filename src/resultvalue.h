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
public:	
	//Creates a new long value
	ResultValue(long value);

	//Creates a new double value
	ResultValue(double value);

	//Crate a new result value
	ResultValue();

	//Returns the type of the value
	ResultValueType type() const;

	//Returns the long value
	long longValue() const;

	//Returns the double value
	double doubleValue() const;

	//Compare operators
	bool operator==(const ResultValue& rhs) const;
	bool operator!=(const ResultValue& rhs) const;
	bool operator<=(const ResultValue& rhs) const;

	double operator+(double value) const;
};

std::ostream& operator<<(std::ostream& os, ResultValue value);