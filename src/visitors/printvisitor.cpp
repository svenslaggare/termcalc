#include "printvisitor.h"
#include "predicatevisitor.h"
#include "../calcengine.h"

PrintVisitor::PrintVisitor(CalcEngine& calcEngine, std::ostream& stream)
	: mCalcEngine(calcEngine), mStream(stream) {

}

std::string PrintVisitor::toString(CalcEngine& calcEngine, Expression* expression) {
	std::stringstream stream;
	PrintVisitor printVisitor(calcEngine, stream);
	expression->accept(printVisitor, expression);
	return stream.str();
}

bool PrintVisitor::needParentheses(Expression* expression, OperatorChar op) {
	if (auto binaryOperatorExpression = dynamic_cast<BinaryOperatorExpression*>(expression)) {
		Operator inputOperator;
		Operator nodeOperator;
		if (mCalcEngine.getBinaryOperator(op, inputOperator)
			&& mCalcEngine.getBinaryOperator(binaryOperatorExpression->op(), nodeOperator)) {
			return inputOperator.precedence() > nodeOperator.precedence();
		}
	}

	return false;
}

void PrintVisitor::visit(Expression* parent, NumberExpression* expression) {
	mStream << expression->value().toString();
}

void PrintVisitor::visit(Expression* parent, VariableExpression* expression) {
	mStream << expression->name();
}

void PrintVisitor::visit(Expression* parent, FunctionCallExpression* expression) {
	mStream << expression->name() << "(";
	bool isFirst = true;

	for (std::size_t i = 0; i < expression->numArguments(); i++) {
		if (!isFirst) {
			mStream << ", ";
		}

		expression->getArgument(i)->accept(*this, expression);
		isFirst = false;
	}

	mStream << ")";
}

void PrintVisitor::visit(Expression* parent, BinaryOperatorExpression* expression) {
	auto needLeftSideParentheses = PredicateVisitor::apply(
		[&](Expression* innerExpression) { return needParentheses(innerExpression, expression->op()); },
		expression->leftHandSide());

	auto needRightSideParentheses = PredicateVisitor::apply(
		[&](Expression* innerExpression) { return needParentheses(innerExpression, expression->op()); },
		expression->rightHandSide());

	if (needLeftSideParentheses) {
		mStream << "(";
	}

	expression->leftHandSide()->accept(*this, expression);

	if (needLeftSideParentheses) {
		mStream << ")";
	}

	mStream << expression->op().toString();

	if (needRightSideParentheses) {
		mStream << "(";
	}

	expression->rightHandSide()->accept(*this, expression);

	if (needRightSideParentheses) {
		mStream << ")";
	}
}

void PrintVisitor::visit(Expression* parent, UnaryOperatorExpression* expression) {
	mStream << expression->op().toString();
	expression->operand()->accept(*this, expression);
}
