#pragma once
#include "resultvalue.h"
#include "operator.h"
#include "numbertype.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class Environment;

//Represents the calculation engine
class CalcEngine {
public:
	using BinaryOperators = std::unordered_map<OperatorChar, Operator>;
private:
	ResultValueType mEvalMode;	

	BinaryOperators mBinaryOperators;
	std::unordered_map<char, Operator> mUnaryOperators;

	std::vector<std::unique_ptr<NumberType>> mNumberTypes;

	//Applies the given binary operator
	ResultValue binaryOperator(NumberOperators op, ResultValueType evalMode, ResultValue x, ResultValue y);

	//Applies the given unary operator
	ResultValue unaryOperator(NumberOperators op, ResultValueType evalMode, ResultValue x);
public:
	//Creates a new calc engine
	CalcEngine();

	//Returns the binary operators
	const BinaryOperators& binaryOperators() const;

	//Returns the unary operators
	const std::unordered_map<char, Operator>& unaryOperators() const;

	//Returns the eval mode
	ResultValueType evalMode() const;

	//Sets the eval mode
	void setEvalMode(ResultValueType evalMode);

	//Evaluates the given expression
	ResultValue eval(std::string expressionString);
	ResultValue eval(std::string expressionString, Environment& env);
};