#pragma once
#include "operator.h"
#include "function.h"
#include "resultvalue.h"
#include <stack>
#include <vector>
#include <memory>
#include <string>

using EvalStack = std::stack<ResultValue>;
class Environment;

//Represents an expression
class Expression {
public:
	Expression() {}
	virtual ~Expression() {}

	//Returns the current expression as a string
	virtual std::string toString() = 0;

	//Evaluates the current expression
	virtual void evaluate(Environment& env, EvalStack& evalStack) = 0;
};

//Represents a double expression
class DoubleExpression : public Expression {
private:
	double mValue;
public:
	//Creates a new double expression
	DoubleExpression(double value);

	virtual std::string toString() override;
	virtual void evaluate(Environment& env, EvalStack& evalStack) override;
};

//Represents an int64 expression
class Int64Expression : public Expression {
private:
	std::int64_t mValue;
public:
	//Creates a new int64 expression
	Int64Expression(std::int64_t value);

	virtual std::string toString() override;
	virtual void evaluate(Environment& env, EvalStack& evalStack) override;
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
	virtual void evaluate(Environment& env, EvalStack& evalStack) override;
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
	virtual void evaluate(Environment& env, EvalStack& evalStack) override;
};

//Represents a binary operator expression
class BinaryOperatorExpression : public Expression {
private:
	Operator mOp;
	std::unique_ptr<Expression> mLHS;
	std::unique_ptr<Expression> mRHS;
public:
	//Creates a new binary operator expression
	BinaryOperatorExpression(Operator op, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

	virtual std::string toString() override;
	virtual void evaluate(Environment& env, EvalStack& evalStack) override;
};

//Represents a unary operator expression
class UnaryOperatorExpression : public Expression {
private:
	Operator mOp;
	std::unique_ptr<Expression> mOperand;
public:
	//Creates a new binary operator expression
	UnaryOperatorExpression(Operator op, std::unique_ptr<Expression> operand);

	virtual std::string toString() override;
	virtual void evaluate(Environment& env, EvalStack& evalStack) override;
};