#include "parser.h"
#include "expression.h"
#include <cmath>

Parser::Parser(std::vector<Token> tokens)
	: mTokens(tokens), mTokenIndex(-1) {

	mBinaryOperators = {
		{ '^', Operator('^', 4, OperatorAssociativity::RIGHT, [](double x, double y) { return pow(x, y); }) },
		{ '*', Operator('*', 3, OperatorAssociativity::LEFT, [](double x, double y) { return x * y; }) },
		{ '/', Operator('/', 3, OperatorAssociativity::LEFT, [](double x, double y) { return x / y; }) },
		{ '%', Operator('%', 3, OperatorAssociativity::LEFT, [](double x, double y) { return (int)x % (int)y; }) },
		{ '+', Operator('+', 2, OperatorAssociativity::LEFT, [](double x, double y) { return x + y; }) },
		{ '-', Operator('-', 2, OperatorAssociativity::LEFT, [](double x, double y) { return x - y; }) },
		{ '=', Operator('=', 1, OperatorAssociativity::RIGHT) }
	};

	mUnaryOperators = {
		{ '-', Operator('-', 5, OperatorAssociativity::LEFT, [](double x) { return -x; }) },
	};

	mFunctions = {
		{ "sin", [](double x) { return sin(x); } },
		{ "cos", [](double x) { return cos(x); } },
		{ "tan", [](double x) { return tan(x); } },
		{ "sqrt", [](double x) { return sqrt(x); } },
		{ "asin", [](double x) { return asin(x); } },
		{ "acos", [](double x) { return acos(x); } },
		{ "atan", [](double x) { return atan(x); } },
		{ "ln", [](double x) { return log(x); } },
		{ "log", [](double x) { return log10(x); } },
	};
}

void Parser::parseError(std::string message) {
	throw std::runtime_error(message);
}

Token& Parser::nextToken() {
	mTokenIndex++;

	if (mTokenIndex >= mTokens.size()) {
		parseError("Reached end of tokens.");
	}

	mCurrentToken = mTokens[mTokenIndex];
	return mCurrentToken;
}

Token& Parser::peekToken(int delta) {
	int nextTokenIndex = mTokenIndex + delta;

	if (nextTokenIndex >= mTokens.size()) {
		parseError("Reached end of tokens.");
	}

	return mTokens[nextTokenIndex];
}

int Parser::getTokenPrecedence() {
	if (mCurrentToken.type() != TokenType::OPERATOR) {
		return -1;
	}

	if (mBinaryOperators.count(mCurrentToken.charValue()) > 0) {
		return mBinaryOperators.at(mCurrentToken.charValue()).precedence();
	} else {
		parseError("'" + std::string { mCurrentToken.charValue() } + "' is not a defined binary operator.");
	}

	return -1;
}

std::unique_ptr<Expression> Parser::parseNumberExpression() {
	double value = mCurrentToken.doubleValue();
	nextToken(); //Consume the number
	return std::unique_ptr<NumberExpression>(new NumberExpression(value));
}

std::unique_ptr<Expression> Parser::parseIdentifierExpression() {
	std::string identifier = mCurrentToken.identifier();

	//Eat the identifier.
	nextToken();

	if (mCurrentToken.type() != TokenType::LEFT_PARENTHESIS) {
		return std::unique_ptr<VariableExpression>(new VariableExpression(identifier));
	}

	//Function call
	nextToken(); //Eat the '('
	std::vector<std::unique_ptr<Expression>> arguments;

	if (mCurrentToken.type() != TokenType::RIGHT_PARENTHESIS) {
		while (true) {
			auto arg = parseExpression();

			if (arg == nullptr) {
				return arg;
			}

			arguments.push_back(std::move(arg));

			if (mCurrentToken.type() == TokenType::RIGHT_PARENTHESIS) {
				break;
			}

			if (mCurrentToken.type() != TokenType::COMMA) {
				parseError("Expected ',' or ')' in argument list.");
			}

			nextToken();
		}
	}

	//Eat the ')'
	nextToken();

	if (mFunctions.count(identifier) == 0) {
		parseError("'" + identifier + "' is not a defined function.");
	}

	if (arguments.size() != 1) {
		parseError("Expected 1 arguments but got " + std::to_string(arguments.size()));
	}

	return std::unique_ptr<FunctionCallExpression>(
		new FunctionCallExpression(identifier, std::move(arguments), mFunctions.at(identifier)));
}

std::unique_ptr<Expression> Parser::parseParenthesisExpression() {
	nextToken(); //Eat the '('

	auto expr = parseExpression();

	if (expr == nullptr) {
		return expr;
	}

	if (mCurrentToken.type() != TokenType::RIGHT_PARENTHESIS) {
		parseError("Expected ').'");
	}

	nextToken(); //Eat the ')'

	return expr;
}

std::unique_ptr<Expression> Parser::parsePrimaryExpression() {
	switch (mCurrentToken.type()) {
	case TokenType::NUMBER:
		return parseNumberExpression();
	case TokenType::IDENTIFIER:
		return parseIdentifierExpression();
	case TokenType::LEFT_PARENTHESIS:
		return parseParenthesisExpression();
	default:
		return std::unique_ptr<Expression>();
	}	
}

std::unique_ptr<Expression> Parser::parseBinaryOpRHS(int precedence, std::unique_ptr<Expression> lhs) {
	while (true) {
		//If this is a bin op, find its precedence
		int tokPrec = getTokenPrecedence();

		//If this is a binary operator that binds as least as tightly as the current operator, consume it, otherwise we are done.
		if (tokPrec < precedence) {
			return lhs;
		}

		char opChar = mCurrentToken.charValue();
		Operator op = mBinaryOperators.at(opChar); 

		nextToken(); //Eat the operator

		//Parse the unary expression after the binary operator
		auto rhs = parseUnaryExpression();

		if (rhs == nullptr) {
			return rhs;
		}

		//If the binary operator binds less tightly with RHS than the operator after RHS, let the pending operator take RHS as its LHS
		int nextPrec = getTokenPrecedence();
		if (tokPrec < nextPrec) {
			rhs = parseBinaryOpRHS(tokPrec + 1, std::move(rhs));

			if (rhs == nullptr) {
				return rhs;
			}
		}

		//Merge LHS and RHS
		lhs = std::move(std::unique_ptr<BinaryOperatorExpression>(
			new BinaryOperatorExpression(op, std::move(lhs), std::move(rhs))));
	}
}

std::unique_ptr<Expression> Parser::parseUnaryExpression() {
	//If the current token isn't an operator, is must be a primary expression
	if (mCurrentToken.type() != TokenType::OPERATOR) {
		return parsePrimaryExpression();
	}

	//If this is a unary operator, read it.
	char opChar = mCurrentToken.charValue();
	nextToken(); //Eat the operator

	auto operand = parseUnaryExpression();

	if (operand != nullptr) {
		if (mUnaryOperators.count(opChar) == 0) {
			parseError("'" + std::string { opChar } + "' is not a defined unary operator.");
		}

		return std::unique_ptr<UnaryOperatorExpression>(
			new UnaryOperatorExpression(mUnaryOperators.at(opChar), std::move(operand)));
	} 

	return operand;
}


std::unique_ptr<Expression> Parser::parseExpression() {
	auto lhs = parseUnaryExpression();

	if (lhs == nullptr) {
		return lhs;
	}

	return parseBinaryOpRHS(0, std::move(lhs));
}

std::unique_ptr<Expression> Parser::parse() {
	nextToken();
	return parseExpression();
}