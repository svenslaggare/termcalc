#pragma once
#include "../core/resultvalue.h"
#include <string>
#include <functional>
#include <vector>
#include <iostream>
#include <memory>

using FunctionArguments = std::vector<ResultValue>;
using ExternalFunction = std::function<ResultValue (FunctionArguments)>;

class Environment;
class CalculationEngine;
class Expression;
	
//Represents an user defined function
class UserFunction {
private:
	std::vector<std::string> mParameters;
	std::unique_ptr<Expression> mBody;
public:
	//Creates a new user defined function
	UserFunction(std::vector<std::string> parameters, std::unique_ptr<Expression> body);

	//Returns the parameters
	const std::vector<std::string>& parameters() const;

	//Returns the body
	Expression* body() const;
};

//Represents a function
class Function {
private:
	std::string mName;
	std::size_t mNumArgs;

	bool mIsUserDefined;
	ExternalFunction mExternalFunction;
	std::shared_ptr<UserFunction> mUserFunction;

	std::string mInfoText;
public:
	//Creates a new external function
	Function(std::string name, std::size_t numParameters, ExternalFunction func, std::string infoText = "");

	//Creates a new user function
	Function(std::string name, std::size_t numParameters, std::shared_ptr<UserFunction> body, std::string infoText = "");

	//Returns the name of the function
	std::string name() const;

	//Returns the number of parameters
	std::size_t numParameters() const;

	//Returns the info text
	std::string infoText() const;

	//Indicates if the function is user defined
	bool isUserDefined() const;

	//Returns the external function (if not user defined)
	ExternalFunction externalFunction() const;

	//Returns the body (if user defined)
	std::shared_ptr<UserFunction> userFunction() const;
};

std::ostream& operator<<(std::ostream& os, const Function& func);