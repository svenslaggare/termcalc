#pragma once
#include "numbertype.h"

/**
 * Represents a complex number type
 */
class ComplexType : public NumberType {
private:
	BinaryOperators mBinaryOperators;
	UnaryOperators mUnaryOperators;
	EnvironmentScope mEnvironment;
public:
	ComplexType(std::ostream& os = std::cout);

	virtual const BinaryOperators& binaryOperators() const override;
	virtual const UnaryOperators& unaryOperators() const override;

	virtual const EnvironmentScope& environment() const override;

	virtual bool isStartOfNumber(const std::string& str, char current, std::size_t index) const override;
	virtual Token parseNumber(std::string& str, char& current, std::size_t& index) const override;
	virtual ResultValue toResultValue(const NumericConstant& numericConstant) const override;
};