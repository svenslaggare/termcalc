#pragma once
#include <string>
#include <iostream>

//The token types
enum class TokenType {
	NUMBER,
	OPERATOR,
	TWO_CHAR_OPERATOR,
	IDENTIFIER,
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
	COMMA,
	END_OF_EXPRESSION
};

//Represents a token
class Token {
private:
	TokenType mType;
	double mDoubleValue;
	std::int64_t mInt64Value;
	char mCharValue;
	char mCharValue2;
	std::string mIdentifier;
public:
	//Creates an empty token
	Token();

	//Creates a new token
	Token(TokenType type);

	//Creates a new double token
	Token(double value);

	//Creates a new int64 token
	Token(std::int64_t value);

	//Creates a new token with a char value
	Token(TokenType type, char value);

	//Creates a new token with two char values
	Token(TokenType type, char value1, char value2);

	//Creates a new identifier token
	Token(std::string identifier);

	//Returns the type of the token
	TokenType type() const;

	//Returns the double value
	double doubleValue() const;

	//Returns the int64 value
	std::int64_t int64Value() const;

	//Returns the char value
	char charValue() const;

	//Returns the second char value
	char charValue2() const;

	//Returns the identifier
	std::string identifier() const;

	bool operator==(const Token& rhs) const;
};

std::ostream& operator<<(std::ostream& ostream, const Token& token);