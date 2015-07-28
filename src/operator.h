#pragma once
enum class OperatorAssociativity {
	LEFT,
	RIGHT
};

//Represents an operator
class Operator {
	char mOp;
	int mPrecedence;
	OperatorAssociativity mAssociativity;
	bool mIsUnary = false;
public:
	//Creates a new operator
	Operator(char op, int precedence, OperatorAssociativity associativity, bool isUnary = false);

	//Returns the operator
	char op() const;

	//Returns the precedence
	int precedence() const;

	//Returns the associativity
	OperatorAssociativity associativity() const;

	//Indicates if an unary operator
	bool isUnary() const;
};