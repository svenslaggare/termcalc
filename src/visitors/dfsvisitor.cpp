#include "dfsvisitor.h"

void DfsVisitor::visit(Expression* parent, NumberExpression* expression) {

}

void DfsVisitor::visit(Expression* parent, VariableExpression* expression) {

}

void DfsVisitor::visit(Expression* parent, FunctionCallExpression* expression) {
	for (std::size_t i = 0; i < expression->numArguments(); i++) {
		expression->getArgument(i)->accept(*this, expression);
	}
}

void DfsVisitor::visit(Expression* parent, BinaryOperatorExpression* expression) {
	expression->leftHandSide()->accept(*this, expression);
	expression->rightHandSide()->accept(*this, expression);
}

void DfsVisitor::visit(Expression* parent, UnaryOperatorExpression* expression) {
	expression->operand()->accept(*this, expression);
}
