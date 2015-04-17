#include "operator.h"

Operator::Operator(char op, int precedence, OperatorAssociativity associativity, ApplyBinaryOperator applyFn)
	: mOp(op), mPrecedence(precedence), mAssociativity(associativity), mBinaryApplyFn(applyFn), mIsUnary(false) {

}

Operator::Operator(char op, int precedence, OperatorAssociativity associativity, ApplyUnaryOperator applyFn)
	: mOp(op), mPrecedence(precedence), mAssociativity(associativity), mUnaryApplyFn(applyFn), mIsUnary(true) {

}

Operator::Operator(char op, int precedence, OperatorAssociativity associativity)
	: mOp(op), mPrecedence(precedence), mAssociativity(associativity),
	  mBinaryApplyFn([](double, double) { return 0; }), mIsUnary(true) {

}

char Operator::op() const {
	return mOp;
}

int Operator::precedence() const {
	return mPrecedence;
}

OperatorAssociativity Operator::associativity() const {
	return mAssociativity;
}

double Operator::apply(double x, double y) const {
	return mBinaryApplyFn(x, y);
}

double Operator::apply(double x) const {
	return mUnaryApplyFn(x);
}

bool Operator::isUnary() const {
	return mIsUnary;
}