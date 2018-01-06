#include "predicatevisitor.h"

PredicateVisitor::PredicateVisitor(ExpressionPredicate predicate)
	: mPredicate(predicate) {

}

bool PredicateVisitor::apply(ExpressionPredicate predicate, Expression* expression) {
	PredicateVisitor predicateVisitor(predicate);
	expression->accept(predicateVisitor, nullptr);
	return predicateVisitor.mHasResult;
}

void PredicateVisitor::visit(Expression* parent, NumberExpression* expression) {
	if (mPredicate(expression)) {
		mHasResult = true;
	}
}

void PredicateVisitor::visit(Expression* parent, VariableExpression* expression) {
	if (mPredicate(expression)) {
		mHasResult = true;
	}
}

void PredicateVisitor::visit(Expression* parent, FunctionCallExpression* expression) {
	if (mHasResult) {
		return;
	}

	DfsVisitor::visit(parent, expression);

	if (mPredicate(expression)) {
		mHasResult = true;
	}
}

void PredicateVisitor::visit(Expression* parent, BinaryOperatorExpression* expression) {
	if (mHasResult) {
		return;
	}

	DfsVisitor::visit(parent, expression);

	if (mPredicate(expression)) {
		mHasResult = true;
	}
}

void PredicateVisitor::visit(Expression* parent, UnaryOperatorExpression* expression) {
	if (mHasResult) {
		return;
	}

	DfsVisitor::visit(parent, expression);

	if (mPredicate(expression)) {
		mHasResult = true;
	}
}
