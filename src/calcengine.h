#pragma once
#include "token.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <functional>

enum class OperatorAssociativity {
	LEFT,
	RIGHT
};

using ApplyBinaryOperator = std::function<double(double, double)>;
using ApplyUnaryOperator = std::function<double(double)>;

//Represents an operator
class Operator {
	char mOp;
	int mPrecedence;
	OperatorAssociativity mAssociativity;
	ApplyBinaryOperator mBinaryApplyFn; 
	ApplyUnaryOperator mUnaryApplyFn; 
	bool mIsUnary = false;
public:
	//Creates a new operator
	Operator(char op, int precedence, OperatorAssociativity associativity, ApplyBinaryOperator applyFn);
	Operator(char op, int precedence, OperatorAssociativity associativity, ApplyUnaryOperator applyFn);
	Operator(char op, int precedence, OperatorAssociativity associativity);

	//Returns the operator
	char op() const;

	//Returns the precedence
	int precedence() const;

	//Returns the associativity
	OperatorAssociativity associativity() const;

	//Applies the operator
	double apply(double x, double y) const;

	//Applies the operator
	double apply(double x) const;

	//Indicates if an unary operator
	bool isUnary() const;
};

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
	std::unordered_map<char, Operator> mBinaryOperators;
	std::unordered_map<char, Operator> mUnaryOperators;
	std::unordered_map<std::string, ApplyUnaryOperator> mFunctions;
	
	//Returns the given operator
	const Operator& getOperator(char op, bool isUnary = false) const;

	//Converts the given tokens into postfix
	std::vector<Token> toPostfix(std::vector<Token> tokens);
public:	
	//Creates a new calc engine
	CalcEngine();

	//Tokenizes the given string
	std::vector<Token> tokenize(std::string str);

	//Evaluates the given expression
	double eval(std::string expression);
	double eval(std::string expression, Environment& env);
};