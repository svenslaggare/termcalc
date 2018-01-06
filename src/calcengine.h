#pragma once
#include "core/resultvalue.h"
#include "core/operator.h"
#include "core/numbertype.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class Environment;

//Represents the calculation engine
class CalcEngine {
private:
	ResultValueType mEvalMode;
	std::unordered_map<ResultValueType, std::unique_ptr<NumberType>> mNumberTypes;
public:
	//Creates a new calc engine
	CalcEngine(std::ostream& os = std::cout);

	//Returns the binary operators
	const BinaryOperators& binaryOperators() const;

	//Tries to get the given binary operator
	bool getBinaryOperator(OperatorChar opChar, Operator& op) const;

	//Returns the unary operators
	const UnaryOperators& unaryOperators() const;

	//Creates a default environment
	Environment defaultEnvironment() const;

	//Returns the eval mode
	ResultValueType evalMode() const;

	//Sets the eval mode
	void setEvalMode(ResultValueType evalMode);

	//Returns the current number type
	NumberType& currentNumberType() const;

	//Evaluates the given expression
	ResultValue eval(std::string expressionString);
	ResultValue eval(std::string expressionString, Environment& env);
};