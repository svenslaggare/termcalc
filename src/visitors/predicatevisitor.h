#pragma once
#include "dfsvisitor.h"
#include <functional>

using ExpressionPredicate = std::function<bool (Expression*)>;

/**
 * Represents a predicate visitor
 */
class PredicateVisitor : public DfsVisitor {
private:
	ExpressionPredicate mPredicate;
	bool mHasResult = false;
public:
	/**
	 * Creates a new predicate visitor
	 * @param predicate The predicate
	 */
	PredicateVisitor(ExpressionPredicate predicate);

	/**
	 * Applies the predicate to the given expression
	 * @param predicate The predicate
	 * @param expression The expression
	 * @return The result of the predicate
	 */
	static bool apply(ExpressionPredicate predicate, Expression* expression);

	void visit(Expression* parent, NumberExpression* expression) override;
	void visit(Expression* parent, VariableExpression* expression) override;
	void visit(Expression* parent, FunctionCallExpression* expression) override;
	void visit(Expression* parent, BinaryOperatorExpression* expression) override;
	void visit(Expression* parent, UnaryOperatorExpression* expression) override;
};