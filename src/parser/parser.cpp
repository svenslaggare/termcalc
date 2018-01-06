#include "parser.h"
#include "../expressions/expression.h"
#include "../calculation/calculationengine.h"
#include <cmath>
#include <unordered_set>

namespace {
	std::unordered_set<char> twoCharOps = { '<', '>' };

	OperatorChar tokenAsOperator(const Token& token) {
		if (token.type() == TokenType::TWO_CHAR_OPERATOR) {
			return OperatorChar(token.charValue(), token.charValue2());
		} else {
			return OperatorChar(token.charValue());
		}
	}
}

std::vector<Token> Tokenizer::tokenize(std::string str, NumberType& numberType) {
	std::vector<Token> tokens;
	for (std::size_t i = 0; i < str.size(); i++) {
		char current = str[i];

		//Skip whitespaces
		if (isspace(current)) {
			continue;
		}

		//Parenthesis
		if (current == '(') {
			tokens.push_back({ TokenType::LEFT_PARENTHESIS });
			continue;
		}

		if (current == ')') {
			tokens.push_back({ TokenType::RIGHT_PARENTHESIS });
			continue;
		}

		//Comma
		if (current == ',') {
			tokens.push_back({ TokenType::COMMA });
			continue;
		}

		//Number
		if (numberType.isStartOfNumber(str, current, i)) {
			tokens.push_back(numberType.parseNumber(str, current, i));
			continue;
		}

		//Identifier
		if (isalpha(current)) {
			std::string identifier { current };

			while (true) {
				std::size_t next = i + 1;

				if (next >= str.size()) {
					break;
				}

				current = str[next];

				if (!(isdigit(current) || isalpha(current))) {
					break;
				}

				identifier += current;
				i = next;
			}

			tokens.push_back(Token(identifier));
			continue;
		}

		//Operator
		if (tokens.size() > 0 && tokens.back().type() == TokenType::OPERATOR
			&& twoCharOps.count(tokens.back().charValue()) > 0) {
			//If the previous token is an operator and the current one also is, upgrade to a two-op char
			tokens.back() = { TokenType::TWO_CHAR_OPERATOR, tokens.back().charValue(), current };
		} else {
			tokens.push_back({ TokenType::OPERATOR, current });
		}
	}

	return tokens;
}

Parser::Parser(std::vector<Token> tokens, const CalculationEngine& calcEngine)
	:  mCalcEngine(calcEngine), mTokens(tokens), mTokenIndex(-1) {

}

void Parser::parseError(std::string message) {
	throw std::runtime_error(message);
}

Token& Parser::nextToken() {
	mTokenIndex++;

	if ((std::size_t)mTokenIndex >= mTokens.size()) {
		parseError("Reached end of tokens.");
	}

	mCurrentToken = mTokens[mTokenIndex];
	return mCurrentToken;
}

Token& Parser::peekToken(int delta) {
	int nextTokenIndex = mTokenIndex + delta;

	if ((std::size_t)nextTokenIndex >= mTokens.size()) {
		parseError("Reached end of tokens.");
	}

	return mTokens[nextTokenIndex];
}

int Parser::getTokenPrecedence() {
	if (mCurrentToken.type() != TokenType::OPERATOR && mCurrentToken.type() != TokenType::TWO_CHAR_OPERATOR) {
		return -1;
	}

	auto op = tokenAsOperator(mCurrentToken);
	if (mCalcEngine.binaryOperators().count(op) > 0) {
		return mCalcEngine.binaryOperators().at(op).precedence();
	} else {
		parseError("'" + op.toString() + "' is not a defined binary operator.");
	}

	return -1;
}

std::unique_ptr<Expression> Parser::parseNumberExpression() {
	auto value = mCurrentToken.numericValue();
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

	return std::unique_ptr<FunctionCallExpression>(
		new FunctionCallExpression(identifier, std::move(arguments)));
}

std::unique_ptr<Expression> Parser::parseParenthesisExpression() {
	nextToken(); //Eat the '('
	auto expr = parseExpression();

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
		parseError("Expected an expression");
		return nullptr;
	}
}

std::unique_ptr<Expression> Parser::parseBinaryOpRHS(int precedence, std::unique_ptr<Expression> lhs) {
	while (true) {
		//If this is a bin op, find its precedence
		int tokPrecedence = getTokenPrecedence();

		//If this is a binary operator that binds at least as tightly as the current operator, consume it, otherwise we are done.
		if (tokPrecedence < precedence) {
			return lhs;
		}

		auto op = tokenAsOperator(mCurrentToken);
		nextToken(); //Eat the operator

		//Parse the unary expression after the binary operator
		auto rhs = parseUnaryExpression();

		//If the binary operator binds less tightly with RHS than the operator after RHS, let the pending operator take RHS as its LHS
		int nextPrecedence = getTokenPrecedence();
		if (tokPrecedence < nextPrecedence) {
			rhs = parseBinaryOpRHS(tokPrecedence + 1, std::move(rhs));
		}

		//Merge LHS and RHS
		lhs = std::unique_ptr<BinaryOperatorExpression>(
			new BinaryOperatorExpression(op, std::move(lhs), std::move(rhs)));
	}
}

std::unique_ptr<Expression> Parser::parseUnaryExpression() {
	//If the current token isn't an operator, is must be a primary expression
	if (mCurrentToken.type() != TokenType::OPERATOR) {
		return parsePrimaryExpression();
	}

	//If this is a unary operator, read it.
	auto op = tokenAsOperator(mCurrentToken);
	nextToken(); //Eat the operator

	auto operand = parseUnaryExpression();

	if (operand != nullptr) {
		if (mCalcEngine.unaryOperators().count(op) == 0) {
			parseError("'" + op.toString() + "' is not a defined unary operator.");
		}

		return std::unique_ptr<UnaryOperatorExpression>(
			new UnaryOperatorExpression(op, std::move(operand)));
	}

	return operand;
}


std::unique_ptr<Expression> Parser::parseExpression() {
	return parseBinaryOpRHS(0, parseUnaryExpression());
}

std::unique_ptr<Expression> Parser::parse() {
	nextToken();
	return parseExpression();
}
