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
	std::unordered_map<std::string, Function> mFunctions;
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

	//Unsets the given variable
	void unset(std::string variable);

	//Returns the value of the given variable
	ResultValue valueOf(std::string variable) const;

	//Returns the functions
	const std::unordered_map<std::string, Function>& functions() const;

	//Defines the given function
	void define(Function function);

	//Returns the evaluation mode
	ResultValueType evalMode() const;

	//Sets the eval mode
	void setEvalMode(ResultValueType evalMode);
};