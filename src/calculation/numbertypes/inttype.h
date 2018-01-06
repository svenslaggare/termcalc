#pragma once
#include "numbertype.h"

/**
 * Represents an int number type
 */
class IntegerType : public NumberType {
private:
	BinaryOperators mBinaryOperators;
	UnaryOperators mUnaryOperators;
	EnvironmentScope mEnvironment;

	//Computes x^n
	std::int64_t power(std::int64_t x, std::int64_t n) const;

	//Computes a^b mod n
	std::int64_t powerMod(std::int64_t a, std::int64_t b, std::int64_t n);

	//Finds the modular multiplicative inverse of x in n.
	std::int64_t modularMultInverse(std::int64_t x, std::int64_t n);

	//Computes the gcd between x and y
	std::int64_t gcd(std::int64_t x, std::int64_t y);

	//Computes the integer square root of x
	std::int64_t sqrt(std::int64_t x);
public:
	IntegerType(std::ostream& os = std::cout);

	virtual const BinaryOperators& binaryOperators() const override;
	virtual const UnaryOperators& unaryOperators() const override;

	virtual const EnvironmentScope& environment() const override;

	virtual bool isStartOfNumber(const std::string& str, char current, std::size_t index) const override;
	virtual Token parseNumber(std::string& str, char& current, std::size_t& index) const override;
	virtual ResultValue toResultValue(const NumericConstant& numericConstant) const override;
};