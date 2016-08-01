#pragma once
#include "operator.h"
#include "function.h"
#include "resultvalue.h"
#include "numericconstant.h"
#include <stack>
#include <vector>
#include <memory>
#include <string>

using EvalStack = std::stack<ResultValue>;
class Environment;
class CalcEngine;

//Represents an expression
class Expression {
public:
	Expression() {}
	virtual ~Expression() {}

	//Returns the current expression as a string
	virtual std::string toString() = 0;

	//Evaluates the current expression
	virtual void evaluate(CalcEngine& calcEngine, Environment& env, EvalStack& evalStack) = 0;
};

//Represents a number expression
class NumberExpression : public Expression {
private:
	NumericConstant mValue;
public:
	//Creates a new number expression
	NumberExpression(NumericConstant value);

	virtual std::string toString() override;
	virtual void evaluate(CalcEngine& calcEngine, Environment& env, EvalStack& evalStack) override;
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

	virtual std::string toString() override;
	virtual void evaluate(CalcEngine& calcEngine, Environment& env, EvalStack& evalStack) override;
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

	virtual std::string toString() override;
	virtual void evaluate(CalcEngine& calcEngine, Environment& env, EvalStack& evalStack) override;
};

//Represents a binary operator expression
class BinaryOperatorExpression : public Expression {
private:
	OperatorChar mOp;
	std::unique_ptr<Expression> mLHS;
	std::unique_ptr<Expression> mRHS;
public:
	//Creates a new binary operator expression
	BinaryOperatorExpression(OperatorChar op, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

	virtual std::string toString() override;
	virtual void evaluate(CalcEngine& calcEngine, Environment& env, EvalStack& evalStack) override;
};

//Represents a unary operator expression
class UnaryOperatorExpression : public Expression {
private:
	OperatorChar mOp;
	std::unique_ptr<Expression> mOperand;
public:
	//Creates a new binary operator expression
	UnaryOperatorExpression(OperatorChar op, std::unique_ptr<Expression> operand);

	virtual std::string toString() override;
	virtual void evaluate(CalcEngine& calcEngine, Environment& env, EvalStack& evalStack) override;
};