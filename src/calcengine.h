#pragma once
#include "resultvalue.h"
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
private:
	ResultValueType mEvalMode;	
public:	
	//Creates a new calc engine
	CalcEngine();

	//Sets the eval mode
	void setEvalMode(ResultValueType evalMode);

	//Evaluates the given expression
	ResultValue eval(std::string expressionString);
	ResultValue eval(std::string expressionString, Environment& env);
};