#pragma once
#include "resultvalue.h"
#include <string>
#include <functional>
#include <vector>
#include <iostream>
#include <memory>

using FnArgs = std::vector<ResultValue>;
using ApplyFunction = std::function<ResultValue(FnArgs)>;
class Environment;
class CalcEngine;
class Expression;
	
//Represents a function body for an user defined function
class FunctionBody {
private:
	std::vector<std::string> mParameters;
	std::unique_ptr<Expression> mBody;
public:
	//Creates a new function body
	FunctionBody(std::vector<std::string> parameters, std::unique_ptr<Expression> body);

	//Returns the parameters
	const std::vector<std::string>& parameters() const;

	//Returns the body
	Expression* body() const;

	std::string toString() const;

	//Applies the function to the given arguments
	ResultValue apply(CalcEngine& calcEngine, Environment& environment, FnArgs args) const;
};

//Represents a function
class Function {
private:
	std::string mName;
	std::size_t mNumArgs;

	bool mIsUserDefined;
	ApplyFunction mFunc;
	std::shared_ptr<FunctionBody> mUserBody;

	std::string mInfoText;
public:
	//Creates a new function
	Function(std::string name, std::size_t numArgs, ApplyFunction func, std::string infoText = "");

	//Creates a new user function
	Function(std::string name, std::size_t numArgs, std::shared_ptr<FunctionBody> body, std::string infoText = "");

	//Returns the name of the function
	std::string name() const;

	//Returns the number of arguments
	std::size_t numArgs() const;

	//Returns the info text
	std::string infoText() const;

	//Indicates if the function is user defined
	bool isUserDefined() const;

	//Returns the body (if user defined)
	std::shared_ptr<FunctionBody> body() const;

	//Applies the function to the given arguments
	ResultValue apply(CalcEngine& calcEngine, Environment& environment, FnArgs args) const;
};

std::ostream& operator<<(std::ostream& os, const Function& func);