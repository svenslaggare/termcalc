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

OperatorChar::Hash_t OperatorChar::HASH = [](const OperatorChar& op) {
	return (37 * op.mIsTwoChars) + (37 * op.mOp1) + (37 * op.mOp2);
};

OperatorChar::Equal_t OperatorChar::EQUAL = [](const OperatorChar& lhs, const OperatorChar& rhs) {
	return lhs == rhs;
};

//Operator
Operator::Operator(OperatorChar op, int precedence, OperatorAssociativity associativity, BinaryOperatorFn applyFn)
	: mOp(op), mPrecedence(precedence), mIsUnary(false), mBinaryFn(applyFn) {

}

Operator::Operator(OperatorChar op, int precedence, OperatorAssociativity associativity, UnaryOperatorFn applyFn)
	: mOp(op), mPrecedence(precedence), mIsUnary(true), mUnaryFn(applyFn) {

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

ResultValue Operator::apply(ResultValueType evalMode, ResultValue lhs, ResultValue rhs) const {
	if (mIsUnary) {
		throw std::runtime_error("Not a binary operator.");
	}

	return mBinaryFn(evalMode, lhs, rhs);
}

ResultValue Operator::apply(ResultValueType evalMode, ResultValue op) const {
	if (!mIsUnary) {
		throw std::runtime_error("Not an unary operator.");
	}

	return mUnaryFn(evalMode, op);
}
