#pragma once
#include "visitor.h"

/**
 * Represents a DFS visitor
 */
class DfsVisitor : public Visitor {
public:
	void visit(Expression* parent, NumberExpression* expression) override;
	void visit(Expression* parent, VariableExpression* expression) override;
	void visit(Expression* parent, FunctionCallExpression* expression) override;
	void visit(Expression* parent, BinaryOperatorExpression* expression) override;
	void visit(Expression* parent, UnaryOperatorExpression* expression) override;
};