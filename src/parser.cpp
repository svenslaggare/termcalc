#include "parser.h"
#include "expression.h"
#include "calcengine.h"
#include <cmath>
#include <unordered_set>

namespace {
	std::unordered_set<char> twoCharOps = { '<', '>' };

	//Parses a number
	void parseNumber(std::vector<Token>& tokens, std::string& str, char& current, std::size_t& i) {
		std::string num { current };
		bool hasDecimalPoint = false;
		int base = 10;

		//Check if different base
		if (current == '0' && (i + 1) < str.size()) {
			char baseChar = str[i + 1];

			if (baseChar == 'b') {
				base = 2;
				num = "";
				i++;
			} else if (baseChar == 'x') {
				base = 16;
				num = "";
				i++;
			}
		}

		while (true) {
			std::size_t next = i + 1;

			if (next >= str.size()) {
				break;
			}

			current = std::tolower(str[next]);

			if (current == '.') {
				if (!hasDecimalPoint) {
					if (base == 10) {
						hasDecimalPoint = true;
					} else {
						throw std::runtime_error("Decimal points are only allowed in base 10.");
					}
				} else {
					throw std::runtime_error("The token already contains a decimal point.");
				}
			} else {
				if (base == 2) {
					if (!(current == '0' || current == '1')) {
						break;
					}
				} else if (base == 10) {
					if (!isdigit(current)) {
						break;
					}
				} else if (base == 16) {
					if (!(isdigit(current)
						  || current == 'a' || current == 'b' || current == 'c'
						  || current == 'd' || current == 'e' || current == 'f')) {
						break;
					}
				}
			}

			num += current;
			i = next;
		}

		if (base == 10) {
			tokens.emplace_back(std::stod(num));
		} else {
			tokens.emplace_back(std::stol(num, nullptr, base));
		}
	}
}

std::vector<Token> Tokenizer::tokenize(std::string str) {
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
		if (isdigit(current)) {
			parseNumber(tokens, str, current, i);
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
		if (tokens.size() > 0 && tokens.back().type() == TokenType::OPERATOR && twoCharOps.count(tokens.back().charValue()) > 0) {
			//If the previous token is an operator and the current one is, upgrade to a two-op char 
			tokens.back() = { TokenType::TWO_CHAR_OPERATOR, tokens.back().charValue(), current };
		} else {
			tokens.push_back({ TokenType::OPERATOR, current });
		}
	}

	return tokens;
}

Parser::Parser(std::vector<Token> tokens, const CalcEngine& calcEngine)
	:  mCalcEngine(calcEngine), mTokens(tokens), mTokenIndex(-1) {

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

namespace {
	OperatorChar tokenAsOperator(const Token& token) {
		if (token.type() == TokenType::TWO_CHAR_OPERATOR) {
			return OperatorChar(token.charValue(), token.charValue2());
		} else {
			return OperatorChar(token.charValue());
		}
	}
}

int Parser::getTokenPrecedence() {
	if (mCurrentToken.type() != TokenType::OPERATOR && mCurrentToken.type() != TokenType::TWO_CHAR_OPERATOR) {
		return -1;
	}

	auto op = tokenAsOperator(mCurrentToken);
	if (mCalcEngine.binaryOperators().count(op) > 0) {
		return mCalcEngine.binaryOperators().at(op).precedence();
	} else {
		parseError("'" + op.asString() + "' is not a defined binary operator.");
	}

	return -1;
}

std::unique_ptr<Expression> Parser::parseDoubleExpression() {
	if (mCalcEngine.evalMode() == ResultValueType::FLOAT) {
		double value = mCurrentToken.doubleValue();
		nextToken(); //Consume the number
		return std::unique_ptr<DoubleExpression>(new DoubleExpression(value));
	} else {
		long value = mCurrentToken.longValue();
		nextToken(); //Consume the number
		return std::unique_ptr<LongExpression>(new LongExpression(value));
	}
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

	if (mCalcEngine.functions().count(identifier) == 0) {
		parseError("'" + identifier + "' is not a defined function.");
	}

	auto& func = mCalcEngine.functions().at(identifier);

	if (arguments.size() != func.numArgs()) {
		parseError("Expected " + std::to_string(func.numArgs()) + " arguments but got " + std::to_string(arguments.size()));
	}

	return std::unique_ptr<FunctionCallExpression>(
		new FunctionCallExpression(identifier, std::move(arguments), func));
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
		return parseDoubleExpression();
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

		auto op = mCalcEngine.binaryOperators().at(tokenAsOperator(mCurrentToken)); 
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
		if (mCalcEngine.unaryOperators().count(opChar) == 0) {
			parseError("'" + std::string { opChar } + "' is not a defined unary operator.");
		}

		return std::unique_ptr<UnaryOperatorExpression>(
			new UnaryOperatorExpression(mCalcEngine.unaryOperators().at(opChar), std::move(operand)));
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