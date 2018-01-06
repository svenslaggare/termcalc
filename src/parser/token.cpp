#include "token.h"
#include <cmath>

Token::Token()
	: mType(TokenType::NUMBER),
	  mNumericValue(),
	  mCharValue(0),
	  mCharValue2(0) {

}

Token::Token(TokenType type)
	: mType(type),
	  mNumericValue(),
	  mCharValue(0),
	  mCharValue2(0) {

}

Token::Token(NumericConstant value)
	: mType(TokenType::NUMBER),
	  mNumericValue(value),
	  mCharValue(0),
	  mCharValue2(0) {

}

Token::Token(TokenType type, char value)
	: mType(type),
	  mNumericValue(),
	  mCharValue(value),
	  mCharValue2(0) {

}

Token::Token(TokenType type, char value1, char value2)
	: mType(type),
	  mNumericValue(),
	  mCharValue(value1),
	  mCharValue2(value1) {

}

Token::Token(std::string identifier)
	: mType(TokenType::IDENTIFIER),
	  mNumericValue(),
	  mCharValue(0),
	  mCharValue2(0),
	  mIdentifier(identifier) {

}

TokenType Token::type() const {
	return mType;
}

const NumericConstant& Token::numericValue() const {
	return mNumericValue;
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

	if (mNumericValue != rhs.numericValue()) {
		return false;
	}

	if (mCharValue != rhs.mCharValue) {
		return false;
	}

	if (mCharValue2 != rhs.mCharValue2) {
		return false;
	}

	if (mIdentifier != rhs.mIdentifier) {
		return false;
	}

	return true;
}

bool Token::operator!=(const Token& rhs) const {
	return !(*this == rhs);
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
	switch (token.type()) {
		case TokenType::NUMBER:
			os << token.numericValue().toString();
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
