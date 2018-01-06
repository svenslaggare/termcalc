#include "expression.h"
#include "../calculation/calculationengine.h"
#include "../calculation/environment.h"
#include "../calculation/function.h"
#include "../visitors/visitor.h"

#include <cmath>
#include <unordered_set>
#include <sstream>

//Number expression
NumberExpression::NumberExpression(NumericConstant value)
	: mValue(value) {

}

const NumericConstant& NumberExpression::value() const {
	return mValue;
}

void NumberExpression::accept(Visitor& visitor, Expression* parent) {
	visitor.visit(parent, this);
}

//Variable expression
VariableExpression::VariableExpression(std::string name)
	: mName(name) {

}

std::string VariableExpression::name() const {
	return mName;
}

void VariableExpression::accept(Visitor& visitor, Expression* parent) {
	visitor.visit(parent, this);
}

//Function call
FunctionCallExpression::FunctionCallExpression(std::string name, std::vector<std::unique_ptr<Expression>> arguments)
	: mName(name), mArguments(std::move(arguments)) {

}

std::string FunctionCallExpression::name() const {
	return mName;
}

std::size_t FunctionCallExpression::numArguments() const {
	return mArguments.size();
}

Expression* FunctionCallExpression::getArgument(std::size_t index) const {
	if (index >= mArguments.size()) {
		throw std::runtime_error("Invalid argument index.");
	}

	return mArguments[index].get();
}

void FunctionCallExpression::accept(Visitor& visitor, Expression* parent) {
	visitor.visit(parent, this);
}

//Binary operator expression
BinaryOperatorExpression::BinaryOperatorExpression(OperatorChar op,
												   std::unique_ptr<Expression> lhs,
												   std::unique_ptr<Expression> rhs)
	: mOp(op),
	  mLeftHandSide(std::move(lhs)),
	  mRightHandSide(std::move(rhs)) {

}

OperatorChar BinaryOperatorExpression::op() const {
	return mOp;
}

Expression* BinaryOperatorExpression::leftHandSide() const {
	return mLeftHandSide.get();
}

Expression* BinaryOperatorExpression::rightHandSide() const {
	return mRightHandSide.get();
}

Expression* BinaryOperatorExpression::releaseRightHandSide() {
	return mRightHandSide.release();
}

void BinaryOperatorExpression::accept(Visitor& visitor, Expression* parent) {
	visitor.visit(parent, this);
}

//Unary expression
UnaryOperatorExpression::UnaryOperatorExpression(OperatorChar op, std::unique_ptr<Expression> operand)
	: mOp(op), mOperand(std::move(operand)) {

}

OperatorChar UnaryOperatorExpression::op() const {
	return mOp;
}

Expression* UnaryOperatorExpression::operand() const {
	return mOperand.get();
}



void UnaryOperatorExpression::accept(Visitor& visitor, Expression* parent) {
	visitor.visit(parent, this);
}
