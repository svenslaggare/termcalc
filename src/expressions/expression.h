#pragma once
#include "../core/operator.h"
#include "../core/function.h"
#include "../core/resultvalue.h"
#include "../core/numericconstant.h"
#include <stack>
#include <vector>
#include <memory>
#include <string>

class Environment;
class CalcEngine;
class Visitor;

//Represents an expression
class Expression {
public:
	Expression() {}
	virtual ~Expression() {}

	//Accepts the given visitor
	virtual void accept(Visitor& visitor, Expression* parent) = 0;
};

//Represents a number expression
class NumberExpression : public Expression {
private:
	NumericConstant mValue;
public:
	//Creates a new number expression
	NumberExpression(NumericConstant value);

	//Returns the value of the expression
	const NumericConstant& value() const;

	virtual void accept(Visitor& visitor, Expression* parent) override;
};

//Represents a variable expression
class VariableExpression : public Expression {
private:
	std::string mName;
public:
	//Creates a new variable expression
	VariableExpression(std::string name);

	//Returns the name of the variable
	std::string name() const;

	virtual void accept(Visitor& visitor, Expression* parent) override;
};

//Represents a function call expression
class FunctionCallExpression : public Expression {
private:
	std::string mName;
	std::vector<std::unique_ptr<Expression>> mArguments;
public:
	//Creates a new function call expression
	FunctionCallExpression(std::string name, std::vector<std::unique_ptr<Expression>> arguments);

	//Returns the name of the function to call
	std::string name() const;

	//Returns the number of arguments
	std::size_t numArguments() const;

	//Returns the given argument
	Expression* getArgument(std::size_t index) const;

	virtual void accept(Visitor& visitor, Expression* parent) override;
};

//Represents a binary operator expression
class BinaryOperatorExpression : public Expression {
private:
	OperatorChar mOp;
	std::unique_ptr<Expression> mLeftHandSide;
	std::unique_ptr<Expression> mRightHandSide;
public:
	//Creates a new binary operator expression
	BinaryOperatorExpression(OperatorChar op, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

	//Returns the operator
	OperatorChar op() const;

	//Returns the left hand side
	Expression* leftHandSide() const;

	//Returns the right hand side
	Expression* rightHandSide() const;

	//Releases ownership of the right hand side
	Expression* releaseRightHandSide();

	virtual void accept(Visitor& visitor, Expression* parent) override;
};

//Represents a unary operator expression
class UnaryOperatorExpression : public Expression {
private:
	OperatorChar mOp;
	std::unique_ptr<Expression> mOperand;
public:
	//Creates a new binary operator expression
	UnaryOperatorExpression(OperatorChar op, std::unique_ptr<Expression> operand);

	//Returns the operator
	OperatorChar op() const;

	//Returns the operand
	Expression* operand() const;

	virtual void accept(Visitor& visitor, Expression* parent) override;
};