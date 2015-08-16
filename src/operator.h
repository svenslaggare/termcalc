#pragma once
#include <functional>

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
	OperatorChar mOp;
	int mPrecedence;
	OperatorAssociativity mAssociativity;
	bool mIsUnary = false;
public:
	//Creates a new operator
	Operator(OperatorChar op, int precedence, OperatorAssociativity associativity, bool isUnary = false);

	//Returns the operator character
	OperatorChar op() const;

	//Returns the precedence
	int precedence() const;

	//Returns the associativity
	OperatorAssociativity associativity() const;

	//Indicates if an unary operator
	bool isUnary() const;
};