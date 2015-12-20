#pragma once
#include "token.h"
#include "resultvalue.h"
#include <vector>
#include <memory>

class Token;
class Expression;
class CalcEngine;
class NumberType;

//Represents a tokenizer
namespace Tokenizer {
	//Tokenizes the given string
	std::vector<Token> tokenize(std::string str, NumberType& numberType);
};

//Represents a parser
class Parser {
private:
	const CalcEngine& mCalcEngine;

	std::vector<Token> mTokens;
	Token mCurrentToken;
	int mTokenIndex;

	//Signals that a parse error has occurred
	void parseError(std::string message);

	//Advances to the next token
	Token& nextToken();

	//Returns the next token
	Token& peekToken(int delta = 1);

	//Returns the precedence for the current token or -1 if not an operator
	int getTokenPrecedence();

	//Parses a number expression
	std::unique_ptr<Expression> parseNumberExpression();

	//Parses an identifier expression
	std::unique_ptr<Expression> parseIdentifierExpression();

	//Parses a primary expression
	std::unique_ptr<Expression> parsePrimaryExpression();

	//Parses a parenthesis expression
	std::unique_ptr<Expression> parseParenthesisExpression();

	//Parses the right hand side of an binary op expression
	std::unique_ptr<Expression> parseBinaryOpRHS(int precedence, std::unique_ptr<Expression> lhs);

	//Parses a unary expression
	std::unique_ptr<Expression> parseUnaryExpression();

	//Parses an expression
	std::unique_ptr<Expression> parseExpression();
public:
	//Creates a new parser
	Parser(std::vector<Token> tokens, const CalcEngine& calcEngine);

	//Parses the tokens
	std::unique_ptr<Expression> parse();
};