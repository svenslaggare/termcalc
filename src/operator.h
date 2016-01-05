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
};

//Custom hash
namespace std {
	template <>
	struct hash<OperatorChar> {
		std::size_t operator()(const OperatorChar& op) const {
			return (37 * (std::size_t)op.isTwoChars()) + (37 * (std::size_t)op.op1()) + (37 * (std::size_t)op.op2());
		}
	};
}

//Represents an operator
class Operator {
public:
	using BinaryOperatorFn = std::function<ResultValue (ResultValue, ResultValue)>;
	using UnaryOperatorFn = std::function<ResultValue (ResultValue)>;
private:
	OperatorChar mOp;
	int mPrecedence;
	OperatorAssociativity mAssociativity;
	bool mIsUnary = false;
	BinaryOperatorFn mBinaryFn;
	UnaryOperatorFn mUnaryFn;

	//Creates a new operator
	Operator(OperatorChar op, int precedence, OperatorAssociativity associativity, bool isUnary, BinaryOperatorFn binaryFn, UnaryOperatorFn unaryFn);
public:
	//Creates a new binary operator
	static Operator binary(OperatorChar op, int precedence, OperatorAssociativity associativity, BinaryOperatorFn applyFn);

	//Creates a new unary operator
	static Operator unary(OperatorChar op, int precedence, OperatorAssociativity associativity, UnaryOperatorFn applyFn);

	//Returns the operator character
	OperatorChar op() const;

	//Returns the precedence
	int precedence() const;

	//Returns the associativity
	OperatorAssociativity associativity() const;

	//Indicates if an unary operator
	bool isUnary() const;

	//Applies the operator to the given values. Fails if not a binary operator.
	ResultValue apply(ResultValue lhs, ResultValue rhs) const;

	//Applies the operator to the given value. Fails if not an unary operator.
	ResultValue apply(ResultValue op) const;
};