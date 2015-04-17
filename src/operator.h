#pragma once
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