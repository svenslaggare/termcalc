#pragma once
#include <functional>
#include <unordered_map>
#include "resultvalue.h"
#include "operator.h"
#include "token.h"
#include "environment.h"

using BinaryOperators = std::unordered_map<OperatorChar, Operator>;
using UnaryOperators = std::unordered_map<OperatorChar, Operator>;

//Represents a number type
class NumberType {
public:
	virtual ~NumberType() {}

	//Returns the binary operators
	virtual const BinaryOperators& binaryOperators() const = 0;

	//Returns the unary operators
	virtual const UnaryOperators& unaryOperators() const = 0;

	//Returns the environment scope
	virtual const EnvironmentScope& environment() const = 0;

	//Indicates if the given char is the start of a number
	virtual bool isStartOfNumber(const std::string& str, char current, std::size_t index) const = 0;

	//Parses the given number
	virtual	Token parseNumber(std::string& str, char& current, std::size_t& index) const = 0;

	//Converts the given string to a numeric constant
	NumericConstant toNumericConstant(std::string str) const;

	//Converts the given numeric constant to result value
	virtual ResultValue toResultValue(const NumericConstant& numericConstant) const = 0;
};

//Integer type
class IntegerType : public NumberType {
private:
	BinaryOperators mBinaryOperators;
	UnaryOperators mUnaryOperators;
	EnvironmentScope mEnvironment;

	//Computes x^n
	std::int64_t power(std::int64_t x, std::int64_t n) const;

	//Computes a^b mod n
	std::int64_t powerMod(std::int64_t a, std::int64_t b, std::int64_t n);

	//Finds the modular multiplicative inverse of x in n.
	std::int64_t modularMultInverse(std::int64_t x, std::int64_t n);

	//Computes the gcd between x and y
	std::int64_t gcd(std::int64_t x, std::int64_t y);

	//Computes the integer square root of x
	std::int64_t sqrt(std::int64_t x);
public:
	IntegerType(std::ostream& os = std::cout);

	virtual const BinaryOperators& binaryOperators() const override;
	virtual const UnaryOperators& unaryOperators() const override;

	virtual const EnvironmentScope& environment() const override;

	virtual bool isStartOfNumber(const std::string& str, char current, std::size_t index) const override;
	virtual Token parseNumber(std::string& str, char& current, std::size_t& index) const override;
	virtual ResultValue toResultValue(const NumericConstant& numericConstant) const override;
};

//Float type
class FloatType : public NumberType {
private:
	BinaryOperators mBinaryOperators;
	UnaryOperators mUnaryOperators;
	EnvironmentScope mEnvironment;
public:
	FloatType();

	virtual const BinaryOperators& binaryOperators() const override;
	virtual const UnaryOperators& unaryOperators() const override;

	virtual const EnvironmentScope& environment() const override;

	virtual bool isStartOfNumber(const std::string& str, char current, std::size_t index) const override;
	virtual Token parseNumber(std::string& str, char& current, std::size_t& index) const override;
	virtual ResultValue toResultValue(const NumericConstant& numericConstant) const override;
};

//Complex type
class ComplexType : public NumberType {
private:
	BinaryOperators mBinaryOperators;
	UnaryOperators mUnaryOperators;
	EnvironmentScope mEnvironment;
public:
	ComplexType(std::ostream& os = std::cout);

	virtual const BinaryOperators& binaryOperators() const override;
	virtual const UnaryOperators& unaryOperators() const override;

	virtual const EnvironmentScope& environment() const override;

	virtual bool isStartOfNumber(const std::string& str, char current, std::size_t index) const override;
	virtual Token parseNumber(std::string& str, char& current, std::size_t& index) const override;
	virtual ResultValue toResultValue(const NumericConstant& numericConstant) const override;
};