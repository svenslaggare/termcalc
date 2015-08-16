#pragma once
#include "resultvalue.h"
#include "operator.h"
#include <string>
#include <unordered_map>

class Environment;

//Represents the calculation engine
class CalcEngine {
public:
	using BinaryOperators = std::unordered_map<OperatorChar, Operator, OperatorChar::Hash_t, OperatorChar::Equal_t>;
private:
	ResultValueType mEvalMode;	

	BinaryOperators mBinaryOperators;
	std::unordered_map<char, Operator> mUnaryOperators;
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