#include "operator.h"

//Operator char
OperatorChar::OperatorChar(char op)
	: mIsTwoChars(false), mOp1(op), mOp2(0) {

}

OperatorChar::OperatorChar(char op1, char op2)
	: mIsTwoChars(true), mOp1(op1), mOp2(op2) {

}

bool OperatorChar::isTwoChars() const {
	return mIsTwoChars;
}

char OperatorChar::op1() const {
	return mOp1;
}

char OperatorChar::op2() const {
	return mOp2;
}

std::string OperatorChar::toString() const {
	if (mIsTwoChars) {
		return std::string({ mOp1, mOp2 });
	} else {
		return std::string({ mOp1 });
	}
}

bool OperatorChar::operator==(const OperatorChar& rhs) const {
	return mIsTwoChars == rhs.mIsTwoChars && mOp1 == rhs.mOp1 && mOp2 == rhs.mOp2;
}

bool OperatorChar::operator!=(const OperatorChar& rhs) const {
	return !(*this == rhs);
}

//Operator
Operator::Operator(OperatorChar op, int precedence, OperatorAssociativity associativity, bool isUnary, BinaryOperatorFn binaryFn, UnaryOperatorFn unaryFn)
	: mOp(op), mPrecedence(precedence), mIsUnary(isUnary), mBinaryFn(binaryFn), mUnaryFn(unaryFn) {

}

Operator Operator::binary(OperatorChar op, int precedence, OperatorAssociativity associativity, BinaryOperatorFn applyFn) {
	return Operator(op, precedence, associativity, false, applyFn, {});
}

Operator Operator::unary(OperatorChar op, int precedence, OperatorAssociativity associativity, UnaryOperatorFn applyFn) {
	return Operator(op, precedence, associativity, true, {}, applyFn);
}

OperatorChar Operator::op() const {
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

ResultValue Operator::apply(ResultValue lhs, ResultValue rhs) const {
	if (mIsUnary) {
		throw std::runtime_error("Not a binary operator.");
	}

	return mBinaryFn(lhs, rhs);
}

ResultValue Operator::apply(ResultValue op) const {
	if (!mIsUnary) {
		throw std::runtime_error("Not an unary operator.");
	}

	return mUnaryFn(op);
}
