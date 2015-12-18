#pragma once
#include <functional>
#include <unordered_map>
#include "resultvalue.h"
#include "operator.h"
#include "token.h"

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

	//Parses the given number
	virtual	Token parseNumber(std::string& str, char& current, std::size_t& index) const = 0;
};

//Integer type
class IntegerType : public NumberType {
private:
	BinaryOperators mBinaryOperators;
	UnaryOperators mUnaryOperators;

	//Computes x^n
	std::int64_t power(std::int64_t x, std::int64_t n) const;
public:
	IntegerType();

	virtual const BinaryOperators& binaryOperators() const override;
	virtual const UnaryOperators& unaryOperators() const override;

	virtual Token parseNumber(std::string& str, char& current, std::size_t& index) const override;
};

//Float type
class FloatType : public NumberType {
private:
	BinaryOperators mBinaryOperators;
	UnaryOperators mUnaryOperators;
public:
	FloatType();

	virtual const BinaryOperators& binaryOperators() const override;
	virtual const UnaryOperators& unaryOperators() const override;

	virtual Token parseNumber(std::string& str, char& current, std::size_t& index) const override;
};