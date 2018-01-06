#pragma once
#include "../core/resultvalue.h"
#include "../core/operator.h"
#include "function.h"
#include <string>
#include <unordered_map>

using Variables = std::unordered_map<std::string, ResultValue>;
using Functions = std::unordered_map<std::string, Function>;

//Represents a scope for an environment
class EnvironmentScope {
private:
	Variables mVariables;
	Functions mFunctions;
public:
	//Creates a new scope
	EnvironmentScope();
	EnvironmentScope(Variables variables, std::vector<Function> functions);

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

	//Creates a new environment from the given scopes
	Environment(std::unordered_map<ResultValueType, EnvironmentScope> scopes);

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

	//Returns the given function
	const Function& getFunction(std::string name, std::size_t numArgs);

	//Returns the evaluation mode
	ResultValueType evalMode() const;

	//Sets the eval mode
	void setEvalMode(ResultValueType evalMode);
};