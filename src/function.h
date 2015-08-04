#pragma once
#include "resultvalue.h"
#include <string>
#include <functional>
#include <vector>
#include <iostream>

using FnArgs = std::vector<ResultValue>;
using ApplyFunction = std::function<ResultValue(FnArgs)>;

//Represents a function
class Function {
private:
	std::string mName;
	std::size_t mNumArgs;
	ApplyFunction mFunc;
	std::string mInfoText;
public:
	//Creates a new function
	Function(std::string name, std::size_t numArgs, ApplyFunction func, std::string infoText = "");

	//Returns the name of the function
	std::string name() const;

	//Returns the number of arguments
	std::size_t numArgs() const;

	//Returns the info text
	std::string infoText() const;

	//Applies the function to the given arguments
	ResultValue apply(FnArgs args) const;
};

std::ostream& operator<<(std::ostream& os, const Function& func);