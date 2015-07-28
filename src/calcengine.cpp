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
		{ "pi", ResultValue(3.141592653589793238463) }
	};
}

const std::unordered_map<std::string, ResultValue>& Environment::variables() {
	return mValues;
}

void Environment::set(std::string variable, ResultValue value) {
	mValues[variable] = value;
}

bool Environment::getVariable(std::string variable, ResultValue& value) const {
	if (mValues.count(variable) > 0) {
		value = mValues.at(variable);
		return true;
	} else {
		return false;
	}
}

ResultValue Environment::valueOf(std::string variable) const {
	return mValues.at(variable);
}

//Calc engine
CalcEngine::CalcEngine()
	: mEvalMode(ResultValueType::FLOAT) {

}

void CalcEngine::setEvalMode(ResultValueType evalMode) {
	mEvalMode = evalMode;
}

ResultValue CalcEngine::eval(std::string expressionString) {
	Environment env;
	return eval(expressionString, env);
}

ResultValue CalcEngine::eval(std::string expressionString, Environment& env) {
	//Tokenize
	auto tokens = Tokenizer::tokenize(expressionString);
	tokens.push_back(Token(TokenType::END_OF_EXPRESSION));

	//Parse
	Parser parser(tokens, mEvalMode);
	auto expression = parser.parse();
	EvalStack evalStack;

	//Evaluate
	expression->evaluate(env, evalStack);

	if (evalStack.empty()) {
		throw std::runtime_error("Expected result.");
	}

	return evalStack.top();
}
