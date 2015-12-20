#pragma once
#include "resultvalue.h"
#include "operator.h"
#include "function.h"
#include <string>
#include <unordered_map>

using Variables = std::unordered_map<std::string, ResultValue>;
using Functions = std::unordered_map<std::string, Function>;

//Represents a scope for an environment
class EnvironmentScope {
private:
	Variables mVariables;
	std::unordered_map<std::string, Function> mFunctions;
public:
	//Creates a new scope
	EnvironmentScope(Variables variables, Functions functions);

	//Returns the variables
	Variables& variables();
	const Variables& variables() const;

	//Returns the functions
	Functions& functions();
	const Functions& functions() const;
};

//Represents an environment
class Environment {
private:
	std::unordered_map<ResultValueType, EnvironmentScope> mScopes;
	ResultValueType mEvalMode;

	//Returns the current scope
	EnvironmentScope& currentScope();
	const EnvironmentScope& currentScope() const;
public:
	//Creates a new environment
	Environment();

	//Returns the variables
	const Variables& variables() const;

	//Sets the variable to the given value
	void set(std::string variable, ResultValue value);

	//Gets the given variable
	bool getVariable(std::string variable, ResultValue& value, bool applyConversion = false) const;

	//Unsets the given variable
	void unset(std::string variable);

	//Returns the value of the given variable
	ResultValue valueOf(std::string variable) const;

	//Returns the functions
	const Functions& functions() const;

	//Defines the given function
	void define(Function function);

	//Returns the evaluation mode
	ResultValueType evalMode() const;

	//Sets the eval mode
	void setEvalMode(ResultValueType evalMode);
};