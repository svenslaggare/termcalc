#pragma once
#include "../../core/resultvalue.h"
#include "../../core/operator.h"
#include "../../parser/token.h"
#include "../environment.h"

#include <functional>
#include <unordered_map>

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
