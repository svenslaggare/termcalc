#pragma once
#include <functional>
#include "resultvalue.h"

//The operator associativity
enum class OperatorAssociativity {
	LEFT,
	RIGHT
};

//Represents an operator char
class OperatorChar {
private:
	bool mIsTwoChars;
	char mOp1;
	char mOp2;
public:
	//Creates a new single-character operator
	OperatorChar(char op);

	//Creates a new two-character operator
	OperatorChar(char op1, char op2);

	//Indicates if the current operator is two chars
	bool isTwoChars() const;

	//Returns the first operator character
	char op1() const;

	//Returns the second operator character
	char op2() const;	

	//Returns a string representation of the operator
	std::string toString() const;

	bool operator==(const OperatorChar& rhs) const;
	bool operator!=(const OperatorChar& rhs) const;

	//Custom hash
	using Hash_t = std::function<std::size_t(const OperatorChar& op)>;
	static Hash_t HASH;

	using Equal_t = std::function<bool(const OperatorChar& rhs, const OperatorChar& lhs)>;
	static Equal_t EQUAL;
};

//Represents an operator
class Operator {
public:
	using BinaryOperatorFn = std::function<ResultValue (ResultValueType, ResultValue, ResultValue)>;
	using UnaryOperatorFn = std::function<ResultValue (ResultValueType, ResultValue)>;
private:
	OperatorChar mOp;
	int mPrecedence;
	OperatorAssociativity mAssociativity;
	bool mIsUnary = false;
	UnaryOperatorFn mUnaryFn;
	BinaryOperatorFn mBinaryFn;
public:
	//Creates a new operator
	Operator(OperatorChar op, int precedence, OperatorAssociativity associativity, BinaryOperatorFn applyFn);
	Operator(OperatorChar op, int precedence, OperatorAssociativity associativity, UnaryOperatorFn applyFn);

	//Returns the operator character
	OperatorChar op() const;

	//Returns the precedence
	int precedence() const;

	//Returns the associativity
	OperatorAssociativity associativity() const;

	//Indicates if an unary operator
	bool isUnary() const;

	//Applies the operator to the given values. Fails if not a binary operator.
	ResultValue apply(ResultValueType evalMode, ResultValue lhs, ResultValue rhs) const;

	//Applies the operator to the given value. Fails if not an unary operator.
	ResultValue apply(ResultValueType evalMode, ResultValue op) const;
};