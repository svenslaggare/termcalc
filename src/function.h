#pragma once
#include "resultvalue.h"
#include <string>
#include <functional>
#include <vector>

using FnArgs = std::vector<ResultValue>;
using ApplyFunction = std::function<ResultValue(FnArgs)>;

//Represents a function
class Function {
private:
	std::string mName;
	std::size_t mNumArgs;
	ApplyFunction mFunc;
public:
	//Creates a new function
	Function(std::string name, std::size_t numArgs, ApplyFunction func);

	//Returns the name of the function
	std::string name() const;

	//Returns the number of arguments
	std::size_t numArgs() const;

	//Applies the function to the given arguments
	ResultValue apply(FnArgs args) const;
};