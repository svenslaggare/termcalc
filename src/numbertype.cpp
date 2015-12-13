#include "numbertype.h"
#include <cmath>

ResultValue NumberType::binaryOperator(NumberOperators op, ResultValue x, ResultValue y) {
	switch (op) {
		case NumberOperators::Add:
			return add(x, y);
		case NumberOperators::Sub:
			return sub(x, y);
		case NumberOperators::Mul:
			return mul(x, y);
		case NumberOperators::Div:
			return div(x, y);
		case NumberOperators::Pow:
			return power(x, y);
		case NumberOperators::Rem:
			return reminder(x, y);
		case NumberOperators::LeftShift:
			return leftShift(x, y);
		case NumberOperators::RightShift:
			return rightShift(x, y);
		default:
			return ResultValue();
	}
}

ResultValue NumberType::unaryOperator(NumberOperators op, ResultValue x) {
	switch (op) {
		case NumberOperators::Neg:
			return negate(x);
		case NumberOperators::Inv:
			return invert(x);
		default:
			return ResultValue();
	}
}

//Integer type
IntegerType::IntegerType() {

}

ResultValue IntegerType::add(ResultValue x, ResultValue y) {
	return ResultValue(x.intValue() + y.intValue());
}

ResultValue IntegerType::sub(ResultValue x, ResultValue y) {
	return ResultValue(x.intValue() - y.intValue());
}

ResultValue IntegerType::mul(ResultValue x, ResultValue y) {
	return ResultValue(x.intValue() * y.intValue());
}

ResultValue IntegerType::div(ResultValue x, ResultValue y) {
	return ResultValue(x.intValue() / y.intValue());
}

ResultValue IntegerType::power(ResultValue x, ResultValue y) {
	//Using the squaring algorithm
	auto a = x.intValue();
	auto b = y.intValue();

	if (b == 0) {
		return ResultValue(1L);
	}

	auto x1 = a;
	std::int64_t y1 = 1;
	auto n = b;

	while (n > 1) {
		if (n % 2 == 0) {
			x1 *= x1;
			n /= 2;
		} else {
			y1 *= x1;
			x1 *= x1;
			n = (n - 1) / 2;
		}
	}

	return ResultValue(x1 * y1);
}

ResultValue IntegerType::reminder(ResultValue x, ResultValue y) {
	return ResultValue(x.intValue() % y.intValue());
}

ResultValue IntegerType::leftShift(ResultValue x, ResultValue y) {
	return ResultValue(x.intValue() << y.intValue());
}

ResultValue IntegerType::rightShift(ResultValue x, ResultValue y) {
	return ResultValue(x.intValue() >> y.intValue());
}

ResultValue IntegerType::bitwiseAnd(ResultValue x, ResultValue y) {
	return ResultValue(x.intValue() & y.intValue());
}

ResultValue IntegerType::bitwieseOr(ResultValue x, ResultValue y) {
	return ResultValue(x.intValue() | y.intValue());
}

ResultValue IntegerType::negate(ResultValue x) {
	return ResultValue(-x.intValue());
}

ResultValue IntegerType::invert(ResultValue x) {
	return ResultValue(~x.intValue());
}

//Float type
FloatType::FloatType() {

}

ResultValue FloatType::add(ResultValue x, ResultValue y) {
	return ResultValue(x.floatValue() + y.floatValue());
}

ResultValue FloatType::sub(ResultValue x, ResultValue y) {
	return ResultValue(x.floatValue() - y.floatValue());
}

ResultValue FloatType::mul(ResultValue x, ResultValue y) {
	return ResultValue(x.floatValue() * y.floatValue());
}

ResultValue FloatType::div(ResultValue x, ResultValue y) {
	return ResultValue(x.floatValue() / y.floatValue());
}

ResultValue FloatType::power(ResultValue x, ResultValue y) {
	return ResultValue(pow(x.floatValue(), y.floatValue()));
}

ResultValue FloatType::reminder(ResultValue x, ResultValue y) {
	return ResultValue((double)((std::int64_t)x.floatValue() % (std::int64_t)y.floatValue()));
}

ResultValue FloatType::leftShift(ResultValue x, ResultValue y) {
	return ResultValue((double)((std::int64_t)x.floatValue() << (std::int64_t)y.floatValue()));
}

ResultValue FloatType::rightShift(ResultValue x, ResultValue y) {
	return ResultValue((double)((std::int64_t)x.floatValue() >> (std::int64_t)y.floatValue()));
}

ResultValue FloatType::bitwiseAnd(ResultValue x, ResultValue y) {
	return ResultValue((double)((std::int64_t)x.floatValue() & (std::int64_t)y.floatValue()));
}

ResultValue FloatType::bitwieseOr(ResultValue x, ResultValue y) {
	return ResultValue((double)((std::int64_t)x.floatValue() | (std::int64_t)y.floatValue()));
}

ResultValue FloatType::negate(ResultValue x) {
	return ResultValue(-x.floatValue());
}

ResultValue FloatType::invert(ResultValue x) {
	return ResultValue((double)(~(std::int64_t)x.floatValue()));
}
