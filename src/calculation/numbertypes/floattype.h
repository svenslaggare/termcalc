#pragma once
#include "numbertype.h"

/**
 * Represents a float number type
 */
class FloatType : public NumberType {
private:
	BinaryOperators mBinaryOperators;
	UnaryOperators mUnaryOperators;
	EnvironmentScope mEnvironment;
public:
	FloatType();

	virtual const BinaryOperators& binaryOperators() const override;
	virtual const UnaryOperators& unaryOperators() const override;

	virtual const EnvironmentScope& environment() const override;

	virtual bool isStartOfNumber(const std::string& str, char current, std::size_t index) const override;
	virtual Token parseNumber(std::string& str, char& current, std::size_t& index) const override;
	virtual ResultValue toResultValue(const NumericConstant& numericConstant) const override;
};