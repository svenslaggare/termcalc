#include "token.h"
#include <cmath>

Token::Token()
	: mType(TokenType::NUMBER), mDoubleValue(0), mCharValue(0), mCharValue2(0) {

}

Token::Token(TokenType type)
	: mType(type), mDoubleValue(0.0), mInt64Value(0), mCharValue(0), mCharValue2(0) {

}

Token::Token(double value)
	: mType(TokenType::NUMBER), mDoubleValue(value), mInt64Value(value), mCharValue(0), mCharValue2(0) {

}

Token::Token(std::int64_t value)
	: mType(TokenType::NUMBER), mDoubleValue(value), mInt64Value(value), mCharValue(0), mCharValue2(0) {

}

Token::Token(TokenType type, char value)
	: mType(type), mDoubleValue(0), mInt64Value(0), mCharValue(value), mCharValue2(0) {

}

Token::Token(TokenType type, char value1, char value2)
	: mType(type), mDoubleValue(0), mInt64Value(0), mCharValue(value1), mCharValue2(value1) {

}

Token::Token(std::string identifier)
	: mType(TokenType::IDENTIFIER), mDoubleValue(0), mInt64Value(0), mCharValue(0), mCharValue2(0), mIdentifier(identifier) {

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

	if (mIdentifier != rhs.mIdentifier) {
		return false;
	}

	return true;
}

std::ostream& operator<<(std::ostream& ostream, const Token& token) {
	switch (token.type()) {
		case TokenType::NUMBER:
			ostream << token.doubleValue();
			break;
		case TokenType::OPERATOR:
			ostream << token.charValue();
			break;
		case TokenType::IDENTIFIER:
			ostream << token.identifier();
			break;
		case TokenType::LEFT_PARENTHESIS:
			ostream << "(";
			break;
		case TokenType::RIGHT_PARENTHESIS:
			ostream << ")";
			break;
		case TokenType::COMMA:
			ostream << ",";
			break;
		default:
			break;
	}

	return ostream;
}
