#pragma once

#include <functional>
#include "resultvalue.h"

//The number operators
enum class NumberOperators : char {
	Add,
	Sub,
	Mul,
	Div,
	Pow,
	Rem,
	LeftShift,
	RightShift,
	BitwiseAnd,
	BitwiseOr,
	Neg,
	Inv
};

using BinaryOperator = std::function<ResultValue (ResultValue, ResultValue)>;
using UnaryOperator = std::function<ResultValue (ResultValue)>;

//Represents a number type
class NumberType {
public:
	virtual ~NumberType() {}

	//Arithmetic
	virtual ResultValue add(ResultValue x, ResultValue y) = 0;
	virtual ResultValue sub(ResultValue x, ResultValue y) = 0;
	virtual ResultValue mul(ResultValue x, ResultValue y) = 0;
	virtual ResultValue div(ResultValue x, ResultValue y) = 0;
	virtual ResultValue power(ResultValue x, ResultValue y) = 0;
	virtual ResultValue reminder(ResultValue x, ResultValue y) = 0;

	//Logic
	virtual ResultValue leftShift(ResultValue x, ResultValue y) = 0;
	virtual ResultValue rightShift(ResultValue x, ResultValue y) = 0;
	virtual ResultValue bitwiseAnd(ResultValue x, ResultValue y) = 0;
	virtual ResultValue bitwieseOr(ResultValue x, ResultValue y) = 0;

	//Unary
	virtual ResultValue negate(ResultValue x) = 0;
	virtual ResultValue invert(ResultValue x) = 0;

	//Applies the given binary operator
	ResultValue binaryOperator(NumberOperators op, ResultValue x, ResultValue y);

	//Applies the given unary operator
	ResultValue unaryOperator(NumberOperators op, ResultValue x);
};

//Integer type
class IntegerType : public NumberType {
public:
	IntegerType();

	//Arithmetic
	virtual ResultValue add(ResultValue x, ResultValue y) override;
	virtual ResultValue sub(ResultValue x, ResultValue y) override;
	virtual ResultValue mul(ResultValue x, ResultValue y) override;
	virtual ResultValue div(ResultValue x, ResultValue y) override;
	virtual ResultValue power(ResultValue x, ResultValue y) override;
	virtual ResultValue reminder(ResultValue x, ResultValue y) override;

	//Logic
	virtual ResultValue leftShift(ResultValue x, ResultValue y) override;
	virtual ResultValue rightShift(ResultValue x, ResultValue y) override;
	virtual ResultValue bitwiseAnd(ResultValue x, ResultValue y) override;
	virtual ResultValue bitwieseOr(ResultValue x, ResultValue y) override;

	//Unary
	virtual ResultValue negate(ResultValue x) override;
	virtual ResultValue invert(ResultValue x) override;
};

//Float type
class FloatType : public NumberType {
public:
	FloatType();

	//Arithmetic
	virtual ResultValue add(ResultValue x, ResultValue y) override;
	virtual ResultValue sub(ResultValue x, ResultValue y) override;
	virtual ResultValue mul(ResultValue x, ResultValue y) override;
	virtual ResultValue div(ResultValue x, ResultValue y) override;
	virtual ResultValue power(ResultValue x, ResultValue y) override;
	virtual ResultValue reminder(ResultValue x, ResultValue y) override;

	//Logic
	virtual ResultValue leftShift(ResultValue x, ResultValue y) override;
	virtual ResultValue rightShift(ResultValue x, ResultValue y) override;
	virtual ResultValue bitwiseAnd(ResultValue x, ResultValue y) override;
	virtual ResultValue bitwieseOr(ResultValue x, ResultValue y) override;

	//Unary
	virtual ResultValue negate(ResultValue x) override;
	virtual ResultValue invert(ResultValue x) override;
};