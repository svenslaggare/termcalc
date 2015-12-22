#include "token.h"
#include <cmath>

Token::Token()
	: mType(TokenType::NUMBER),
	  mDoubleValue(0.0),
	  mInt64Value(0),
	  mComplexValue(0.0),
	  mCharValue(0),
	  mCharValue2(0) {

}

Token::Token(TokenType type)
	: mType(type),
	  mDoubleValue(0.0),
	  mInt64Value(0),
	  mComplexValue(0.0),
	  mCharValue(0),
	  mCharValue2(0) {

}

Token::Token(double value)
	: mType(TokenType::NUMBER),
	  mDoubleValue(value),
	  mInt64Value((std::int64_t)value),
	  mComplexValue(value),
	  mCharValue(0),
	  mCharValue2(0) {

}

Token::Token(std::int64_t value)
	: mType(TokenType::NUMBER),
	  mDoubleValue(value),
	  mInt64Value(value),
	  mComplexValue(value),
	  mCharValue(0),
	  mCharValue2(0) {

}

Token::Token(std::complex<double> value)
	: mType(TokenType::NUMBER),
	  mDoubleValue(value.real()),
	  mInt64Value((std::int64_t)value.real()),
	  mComplexValue(value),
	  mCharValue(0),
	  mCharValue2(0) {

}

Token::Token(TokenType type, char value)
	: mType(type),
	  mDoubleValue(0),
	  mInt64Value(0),
	  mCharValue(value),
	  mCharValue2(0) {

}

Token::Token(TokenType type, char value1, char value2)
	: mType(type),
	  mDoubleValue(0),
	  mInt64Value(0),
	  mCharValue(value1),
	  mCharValue2(value1) {

}

Token::Token(std::string identifier)
	: mType(TokenType::IDENTIFIER),
	  mDoubleValue(0),
	  mInt64Value(0),
	  mCharValue(0),
	  mCharValue2(0),
	  mIdentifier(identifier) {

}

TokenType Token::type() const {
	return mType;
}

double Token::doubleValue() const {
	return mDoubleValue;
}

std::int64_t Token::int64Value() const {
	return mInt64Value;
}

std::complex<double> Token::complexValue() const {
	return mComplexValue;
}

char Token::charValue() const {
	return mCharValue;
}

char Token::charValue2() const {
	return mCharValue2;
}

std::string Token::identifier() const {
	return mIdentifier;
}

bool Token::operator==(const Token& rhs) const {
	if (mType != rhs.mType) {
		return false;
	}

	if (std::abs(mDoubleValue - rhs.mDoubleValue) > 0.00000001) {
		return false;
	}

	if (mCharValue != rhs.mCharValue) {
		return false;
	}

	if (mCharValue2 != rhs.mCharValue2) {
		return false;
	}

	if (mInt64Value != rhs.mInt64Value) {
		return false;
	}

	if (mComplexValue != rhs.mComplexValue) {
		return false;
	}

	if (mIdentifier != rhs.mIdentifier) {
		return false;
	}

	return true;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
	switch (token.type()) {
		case TokenType::NUMBER:
			os << token.doubleValue();
			break;
		case TokenType::OPERATOR:
			os << token.charValue();
			break;
		case TokenType::IDENTIFIER:
			os << token.identifier();
			break;
		case TokenType::LEFT_PARENTHESIS:
			os << "(";
			break;
		case TokenType::RIGHT_PARENTHESIS:
			os << ")";
			break;
		case TokenType::COMMA:
			os << ",";
			break;
		default:
			break;
	}

	return os;
}
