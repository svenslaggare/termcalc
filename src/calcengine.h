#pragma once
#include "resultvalue.h"
#include "operator.h"
#include "function.h"
#include <string>
#include <unordered_map>

//Represents an environment
class Environment {
private:
	std::unordered_map<std::string, ResultValue> mValues;
	ResultValueType mEvalMode;
public:
	//Creates a new environment
	Environment();

	//Returns the variables
	const std::unordered_map<std::string, ResultValue>& variables();

	//Sets the variable to the given value
	void set(std::string variable, ResultValue value);

	//Gets the given variable
	bool getVariable(std::string variable, ResultValue& value, bool applyConversion = false) const;

	//Returns the value of the given variable
	ResultValue valueOf(std::string variable) const;

	//Sets the eval mode
	void setEvalMode(ResultValueType evalMode);
};

//Represents the calculation engine
class CalcEngine {
public:
	using BinaryOperators = std::unordered_map<OperatorChar, Operator, OperatorChar::Hash_t, OperatorChar::Equal_t>;
private:
	ResultValueType mEvalMode;	

	BinaryOperators mBinaryOperators;
	std::unordered_map<char, Operator> mUnaryOperators;
	std::unordered_map<std::string, Function> mFunctions;
public:	
	//Creates a new calc engine
	CalcEngine();

	//Returns the binary operators
	const BinaryOperators& binaryOperators() const;

	//Returns the unary operators
	const std::unordered_map<char, Operator>& unaryOperators() const;

	//Returns the functions
	const std::unordered_map<std::string, Function>& functions() const;

	//Returns the eval mode
	ResultValueType evalMode() const;

	//Sets the eval mode
	void setEvalMode(ResultValueType evalMode);

	//Evaluates the given expression
	ResultValue eval(std::string expressionString);
	ResultValue eval(std::string expressionString, Environment& env);
};