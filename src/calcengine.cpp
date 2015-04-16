#include "calcengine.h"
#include <stdexcept>
#include <cmath>
#include <queue>
#include <stack>

//Environment
Environment::Environment() {
	mValues = {
		{ "pi", M_PI }
	};
}

const std::unordered_map<std::string, double>& Environment::variables() {
	return mValues;
}

void Environment::set(std::string variable, double value) {
	mValues[variable] = value;
}

bool Environment::getVariable(std::string variable, double& value) const {
	if (mValues.count(variable) > 0) {
		value = mValues.at(variable);
		return true;
	} else {
		return false;
	}
}

double Environment::valueOf(std::string variable) const {
	return mValues.at(variable);
}

//Calc engine
Operator::Operator(char op, int precedence, OperatorAssociativity associativity, ApplyBinaryOperator applyFn)
	: mOp(op), mPrecedence(precedence), mAssociativity(associativity), mBinaryApplyFn(applyFn), mIsUnary(false) {

}

Operator::Operator(char op, int precedence, OperatorAssociativity associativity, ApplyUnaryOperator applyFn)
	: mOp(op), mPrecedence(precedence), mAssociativity(associativity), mUnaryApplyFn(applyFn), mIsUnary(true) {

}

Operator::Operator(char op, int precedence, OperatorAssociativity associativity)
	: mOp(op), mPrecedence(precedence), mAssociativity(associativity),
	  mBinaryApplyFn([](double, double) { return 0; }), mIsUnary(true) {

}

char Operator::op() const {
	return mOp;
}

int Operator::precedence() const {
	return mPrecedence;
}

OperatorAssociativity Operator::associativity() const {
	return mAssociativity;
}

double Operator::apply(double x, double y) const {
	return mBinaryApplyFn(x, y);
}

double Operator::apply(double x) const {
	return mUnaryApplyFn(x);
}

bool Operator::isUnary() const {
	return mIsUnary;
}

CalcEngine::CalcEngine() {
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

std::vector<Token> CalcEngine::tokenize(std::string str) {
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

		//Number
		if (isdigit(current)) {
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
				tokens.push_back(Token(std::stod(num)));
			} else {
				tokens.push_back(Token(std::stoi(num, nullptr, base)));
			}
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
		tokens.push_back({ TokenType::OPERATOR, current });
	}

	return tokens;
}

//Returns the next token
Token nextToken(std::vector<Token>& tokens, std::size_t& index) {
	index++;

	if (index >= tokens.size()) {
		throw std::runtime_error("Reached end of tokens");
	}

	return tokens[index];
}

const Operator& CalcEngine::getOperator(char op, bool isUnary) const {
	if (isUnary) {
		if (mUnaryOperators.count(op) > 0) {
			return mUnaryOperators.at(op);
		} else {
			throw std::runtime_error("'" + std::string { op } + "' is not a defined unary operator.");
		}
	} else {
		if (mBinaryOperators.count(op) > 0) {
			return mBinaryOperators.at(op);
		} else {
			throw std::runtime_error("'" + std::string { op } + "' is not a defined binary operator.");
		}
	}
}

std::vector<Token> CalcEngine::toPostfix(std::vector<Token> tokens) {
	std::vector<Token> outputQueue;
	std::stack<Token> operatorStack;
	Token prevToken(TokenType::OPERATOR, 0);

	for (int i = 0; i < tokens.size(); ++i)	{
		auto currentToken = tokens[i];

		switch (currentToken.type()) {
		case TokenType::NUMBER:
			outputQueue.push_back(currentToken);
			break;
		case TokenType::IDENTIFIER:
			//Check if function
			if (mFunctions.count(currentToken.identifier())) {
				currentToken.makeFunction();
				operatorStack.push(currentToken);
			} else {
				outputQueue.push_back(currentToken);
			}
			break;
		case TokenType::OPERATOR:
			{
				//Check if a unary operator
				if (prevToken.type() == TokenType::OPERATOR || prevToken.type() == TokenType::LEFT_PARENTHESIS) {
					currentToken.makeUnary();
				}

				auto op1 = getOperator(currentToken.charValue(), currentToken.isUnary());
				
				while (true) {
					//Check for termination
					if (!operatorStack.empty()) {
						auto topToken = operatorStack.top();

						if (topToken.type() == TokenType::OPERATOR) {
							auto op2 = getOperator(topToken.charValue(), currentToken.isUnary());
							if (op1.associativity() == OperatorAssociativity::LEFT) {
								if (op1.precedence() > op2.precedence()) {
									break;
								}
							} else {
								if (op1.precedence() >= op2.precedence()) {
									break;
								}
							}
						} else {
							break;
						}
					} else {
						break;
					}

					//Pop op2 from the operator stack and push it to the output queue
					outputQueue.push_back(operatorStack.top());
					operatorStack.pop();
				}

				operatorStack.push(currentToken);
			}
			break;
		case TokenType::LEFT_PARENTHESIS:
			operatorStack.push(currentToken);
			break;
		case TokenType::RIGHT_PARENTHESIS:
			{
				while (true) {
					//If empty, than the parenthesis does not match
					if (operatorStack.empty()) {
						throw std::runtime_error("The parenthesis does not match.");
					}

					auto op = operatorStack.top();
					operatorStack.pop();
					
					if (op.type() == TokenType::OPERATOR) {
						outputQueue.push_back(op);
					} else if (op.type() == TokenType::LEFT_PARENTHESIS) {
						break;
					}
				}
			}
			break;
		default:
			break;
		}

		prevToken = currentToken;
	}

	//Push the remaining operators to the output queue
	while (!operatorStack.empty()) {
		auto op = operatorStack.top();

		if (op.type() == TokenType::LEFT_PARENTHESIS || op.type() == TokenType::RIGHT_PARENTHESIS) {
			throw std::runtime_error("The parenthsis does not match.");
		}

		operatorStack.pop();
		outputQueue.push_back(op);
	}

	return outputQueue;
}

double CalcEngine::eval(std::string expression) {
	Environment env;
	return eval(expression, env);
}

//Returns the value of the given token
double getValue(Environment& env, Token token) {
	if (token.type() == TokenType::IDENTIFIER) {
		double value;
		if (env.getVariable(token.identifier(), value)) {
			return value;
		} else {
			throw std::runtime_error("There exists no variable called '" + token.identifier() + "'.");
		}
	} else {
		return token.doubleValue();
	}
}

double CalcEngine::eval(std::string expression, Environment& env) {
	//Tokenize
	auto tokens = toPostfix(tokenize(expression));

	//Evaluate the resul
	std::stack<Token> valueStack;
	for (auto token : tokens) {
		switch (token.type()) {
		case TokenType::NUMBER:
			valueStack.push(token);
			break;
		case TokenType::IDENTIFIER:
			{
				if (token.isFunction()) {
					if (valueStack.size() < 1) {
						throw std::runtime_error("Expected one operand.");
					}

					auto op = valueStack.top();
					valueStack.pop();

					//Function
					valueStack.push(mFunctions[token.identifier()](getValue(env, op)));
				} else {
					//Variable
					valueStack.push(token);
				}
			}
			break;
		case TokenType::OPERATOR:
			{
				if (token.isUnary()) {
					if (valueStack.size() < 1) {
						throw std::runtime_error("Expected one operand.");
					}

					auto op1 = valueStack.top();
					valueStack.pop();

					auto op = getOperator(token.charValue(), true);
					valueStack.push(Token(op.apply(getValue(env, op1))));
				} else {
					if (valueStack.size() < 2) {
						throw std::runtime_error("Expected two operands.");
					}

					auto op2 = valueStack.top();
					valueStack.pop();

					auto op1 = valueStack.top();
					valueStack.pop();

					if (token.charValue() == '=') {
						//Assignment
						env.set(op1.identifier(), op2.doubleValue());
						valueStack.push(op2);
					} else {
						auto op = getOperator(token.charValue());
						valueStack.push(Token(op.apply(getValue(env, op1), getValue(env, op2))));
					}
				}
			}
			break;
		default:
			break;
		}
	}

	if (valueStack.empty()) {
		throw std::runtime_error("Expected result.");
	}

	return getValue(env, valueStack.top());
}