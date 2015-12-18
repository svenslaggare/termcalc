#include "numbertype.h"
#include <cmath>

//Integer type
IntegerType::IntegerType() {
	mBinaryOperators = {
		 { '^', Operator('^', 6, OperatorAssociativity::RIGHT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return power(lhs.intValue(), rhs.intValue());
		 }) },
		 { '*', Operator('*', 5, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() * rhs.intValue();
		 }) },
		 { '/', Operator('/', 5, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() / rhs.intValue();
		 }) },
		 { '%', Operator('%', 5, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() % rhs.intValue();
		 }) },
		 { '+', Operator('+', 4, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() + rhs.intValue();
		 }) },
		 { '-', Operator('-', 4, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() - rhs.intValue();
		 }) },
		 { OperatorChar('<', '<'), Operator(OperatorChar('<', '<'), 3, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() << rhs.intValue();
		 }) },
		 { OperatorChar('>', '>'), Operator(OperatorChar('>', '>'), 3, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() >> rhs.intValue();
		 }) },
		 { '|', Operator('|', 2, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() | rhs.intValue();
		 }) },
		 { '&', Operator('&', 2, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() & rhs.intValue();
		 }) },
		 { '=', Operator('=', 1, OperatorAssociativity::RIGHT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return ResultValue();
		 }) }
	 };

	mUnaryOperators = {
		{ '-', Operator('-', 7, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue op) {
			return -op.intValue();
		}) },
		{ '~', Operator('~', 7, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue op) {
			return ~op.intValue();
		}) },
	};
}

std::int64_t IntegerType::power(std::int64_t x, std::int64_t n) const {
	//Using the squaring algorithm
	if (n == 0) {
		return 1;
	}

	std::int64_t y = 1;

	while (n > 1) {
		if (n % 2 == 0) {
			x *= x;
			n /= 2;
		} else {
			y *= x;
			x *= x;
			n = (n - 1) / 2;
		}
	}

	return x * y;
}

const BinaryOperators& IntegerType::binaryOperators() const {
	return mBinaryOperators;
}

const UnaryOperators& IntegerType::unaryOperators() const {
	return mUnaryOperators;
}

//Float type
FloatType::FloatType() {
	mBinaryOperators = {
		 { '^', Operator('^', 6, OperatorAssociativity::RIGHT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return pow(lhs.floatValue(), rhs.floatValue());
		 }) },
		 { '*', Operator('*', 5, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return lhs.floatValue() * rhs.floatValue();
		 }) },
		 { '/', Operator('/', 5, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() / rhs.floatValue();
		 }) },
		 { '+', Operator('+', 4, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return lhs.floatValue() + rhs.floatValue();
		 }) },
		 { '-', Operator('-', 4, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return lhs.floatValue() - rhs.floatValue();
		 }) },
		 { '=', Operator('=', 1, OperatorAssociativity::RIGHT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			 return ResultValue();
		 }) }
	 };

	mUnaryOperators = {
		{ '-', Operator('-', 7, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue op) {
			return -op.floatValue();
		}) }
	};
}

const BinaryOperators& FloatType::binaryOperators() const {
	return mBinaryOperators;
}

const UnaryOperators& FloatType::unaryOperators() const {
	return mUnaryOperators;
}
