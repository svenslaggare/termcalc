#pragma once
#include <iostream>
#include <vector>
#include <complex>

//The type of the result value
enum class ResultValueType {
	NONE,
	FLOAT,
	INTEGER,
	COMPLEX
};

//All types
namespace ResultValueTypes {
	const std::vector<ResultValueType> all = {
		ResultValueType::FLOAT,
		ResultValueType::INTEGER,
		ResultValueType::COMPLEX
	};
}

using Complex = std::complex<double>;

class ResultValue {
private:
	ResultValueType mType;
	std::int64_t mIntValue;
	double mFloatValue;
	Complex mComplexValue;
public:
	//Crate a none result value
	ResultValue();

	//Create a new value of the given type
	ResultValue(ResultValueType type, std::int64_t intValue, double floatValue, Complex complexValue);

	//Creates a new int64 value
	ResultValue(std::int64_t value);

	//Creates a new double value
	ResultValue(double value);

	//Creates a new complex value
	ResultValue(Complex value);

	//Returns the type of the value
	ResultValueType type() const;

	//Returns the int value
	std::int64_t intValue() const;

	//Returns the float value
	double floatValue() const;

	//Returns the complex value
	Complex complexValue() const;

	//Converts the current value to the given type
	ResultValue convertTo(ResultValueType type) const;

	//Returns a string representation
	std::string toString();

	//Compare operators
	bool operator==(const ResultValue& rhs) const;
	bool operator!=(const ResultValue& rhs) const;

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