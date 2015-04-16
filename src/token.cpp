#include "token.h"
#include <cmath>

Token::Token(TokenType type)
	: mType(type), mDoubleValue(0), mCharValue(0) {

}

Token::Token(double value)
	: mType(TokenType::NUMBER), mDoubleValue(value), mCharValue(0) {

}

Token::Token(TokenType type, char value)
	: mType(type), mDoubleValue(0), mCharValue(value) {

}

Token::Token(std::string identifier)
	: mType(TokenType::IDENTIFIER), mDoubleValue(0), mCharValue(0), mIdentifier(identifier) {

}

TokenType Token::type() const {
	return mType;
}

double Token::doubleValue() const {
	return mDoubleValue;
}

char Token::charValue() const {
	return mCharValue;
}

std::string Token::identifier() const {
	return mIdentifier;
}

bool Token::isUnary() const {
	return mIsUnary;
}

void Token::makeUnary() {
	mIsUnary = true;
}

bool Token::isFunction() const {
	return mIsFunction;
}

void Token::makeFunction() {
	mIsFunction = true;
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
	}

	return ostream;
}
