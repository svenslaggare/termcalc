#include "calcengine.h"
#include "parser.h"
#include "expression.h"
#include "operator.h"
#include <stdexcept>
#include <cmath>
#include <queue>
#include <stack>

//Environment
Environment::Environment() {
	mValues = {
		{ "pi", 3.141592653589793238463 }
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
CalcEngine::CalcEngine() {

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
		throw std::runtime_error("Reached end of tokens.");
	}

	return tokens[index];
}

double CalcEngine::eval(std::string expression) {
	Environment env;
	return eval(expression, env);
}

double CalcEngine::eval(std::string expressionString, Environment& env) {
	//Tokenize
	auto tokens = tokenize(expressionString);
	tokens.push_back(Token(TokenType::END_OF_EXPRESSION));

	//Parse
	Parser parser(tokens);
	auto expression = parser.parse();
	std::stack<double> evalStack;

	//Evaluate
	expression->evaluate(env, evalStack);

	if (evalStack.empty()) {
		throw std::runtime_error("Expected result.");
	}

	return evalStack.top();
}
