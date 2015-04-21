#pragma once
#include "operator.h"
#include "function.h"
#include <stack>
#include <vector>
#include <memory>
#include <string>

using EvalStack = std::stack<double>;
class Environment;

//Represents an expression
class Expression {
public:
	virtual ~Expression() {}

	//Evaluates the current expresssion
	virtual void evaluate(Environment& env, EvalStack& evalStack) const = 0;
};

//Represents a number expression
class NumberExpression : public Expression {
private:
	double mValue;
public:
	//Creates a new number expresssion
	NumberExpression(double value);

	virtual void evaluate(Environment& env, EvalStack& evalStack) const override;
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

	virtual void evaluate(Environment& env, EvalStack& evalStack) const override;
};

//Represents a function call expression
class FunctionCallExpression : public Expression {
private:
	std::string mName;
	std::vector<std::unique_ptr<Expression>> mArguments;
	const Function& mFunction;
public:
	//Creates a new function call expression
	FunctionCallExpression(std::string name, std::vector<std::unique_ptr<Expression>> arguments, const Function& function);

	virtual void evaluate(Environment& env, EvalStack& evalStack) const override;
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

	virtual void evaluate(Environment& env, EvalStack& evalStack) const override;
};

//Represents a unary operator expression
class UnaryOperatorExpression : public Expression {
private:
	Operator mOp;
	std::unique_ptr<Expression> mOperand;
public:
	//Creates a new binary operator expression
	UnaryOperatorExpression(Operator op, std::unique_ptr<Expression> operand);

	virtual void evaluate(Environment& env, EvalStack& evalStack) const override;
};