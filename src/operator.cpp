#include "operator.h"

Operator::Operator(char op, int precedence, OperatorAssociativity associativity, bool isUnary)
	: mOp(op), mPrecedence(precedence), mIsUnary(isUnary) {

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

bool Operator::isUnary() const {
	return mIsUnary;
}