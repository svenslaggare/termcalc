#pragma once
#include "token.h"
#include <string>
#include <vector>
#include <unordered_map>

//Represents an environment
class Environment {
private:
	std::unordered_map<std::string, double> mValues;
public:
	//Creates a new environment
	Environment();

	//Returns the variables
	const std::unordered_map<std::string, double>& variables();

	//Sets the variable as the given value
	void set(std::string variable, double value);

	//Gets the given variable
	bool getVariable(std::string variable, double& value) const;

	//Returns the value of the given variable
	double valueOf(std::string variable) const;
};

//Represents the calculation engine
class CalcEngine {
private:

public:	
	//Creates a new calc engine
	CalcEngine();

	//Tokenizes the given string
	std::vector<Token> tokenize(std::string str);

	//Evaluates the given expression
	double eval(std::string expressionString);
	double eval(std::string expressionString, Environment& env);
};