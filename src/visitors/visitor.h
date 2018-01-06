#pragma once
#include "../expressions/expression.h"

/**
 * Represents a visitor
 */
class Visitor {
public:
	virtual ~Visitor() = default;

	//Visits the given expression
	virtual void visit(Expression* parent, NumberExpression* expression);
	virtual void visit(Expression* parent, VariableExpression* expression);
	virtual void visit(Expression* parent, FunctionCallExpression* expression);
	virtual void visit(Expression* parent, BinaryOperatorExpression* expression);
	virtual void visit(Expression* parent, UnaryOperatorExpression* expression);
};