#pragma once
#include <string>
#include <iostream>

//The token types
enum class TokenType {
	NUMBER,
	OPERATOR,
	IDENTIFIER,
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
};

//Represents a token
class Token {
private:
	TokenType mType;
	double mDoubleValue;
	char mCharValue;
	std::string mIdentifier;
	bool mIsUnary = false;
	bool mIsFunction = false;
public:
	//Creates a new token
	Token(TokenType type);

	//Creates a new number token
	Token(double value);

	//Creates a new token with a char value
	Token(TokenType type, char value);

	//Creates a new identifier token
	Token(std::string identifier);

	//Returns the type of the token
	TokenType type() const;

	//Returns the double value
	double doubleValue() const;

	//Returns the char value
	char charValue() const;

	//Returns the identifier
	std::string identifier() const;

	//Indicates if an unary operator
	bool isUnary() const;

	//Marks the operator as unary
	void makeUnary();

	//Indicates if a function
	bool isFunction() const;

	//Marks the identifier a function
	void makeFunction();

	bool operator==(const Token& rhs) const;
};

std::ostream& operator<<(std::ostream& ostream, const Token& token);