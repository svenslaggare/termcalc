#include "calcengine.h"
#include "parser.h"
#include "expression.h"
#include "operator.h"
#include "environment.h"
#include <stdexcept>
#include <queue>
#include <stack>
#include <cmath>

//Calc engine
CalcEngine::CalcEngine()
	: mEvalMode(ResultValueType::FLOAT) {
	mNumberTypes.emplace(ResultValueType::FLOAT, std::unique_ptr<FloatType>(new FloatType));
	mNumberTypes.emplace(ResultValueType::INTEGER, std::unique_ptr<IntegerType>(new IntegerType));
	mNumberTypes.emplace(ResultValueType::COMPLEX, std::unique_ptr<ComplexType>(new ComplexType));
}

NumberType& CalcEngine::currentNumberType() const {
	return *mNumberTypes.at(mEvalMode).get();
}

const BinaryOperators& CalcEngine::binaryOperators() const {
	return currentNumberType().binaryOperators();
}

const UnaryOperators& CalcEngine::unaryOperators() const {
	return currentNumberType().unaryOperators();
}

Environment CalcEngine::defaultEnvironment() const {
	std::unordered_map<ResultValueType, EnvironmentScope> scopes;
	for (auto& numberType : mNumberTypes) {
		scopes.insert({ numberType.first, numberType.second->environment() });
	}

	return Environment(scopes);
}

ResultValueType CalcEngine::evalMode() const {
	return mEvalMode;
}

void CalcEngine::setEvalMode(ResultValueType evalMode) {
	mEvalMode = evalMode;
}

ResultValue CalcEngine::eval(std::string expressionString) {
	auto env = defaultEnvironment();
	return eval(expressionString, env);
}

ResultValue CalcEngine::eval(std::string expressionString, Environment& env) {
	env.setEvalMode(mEvalMode);

	//Tokenize
	auto tokens = Tokenizer::tokenize(expressionString, currentNumberType());
	tokens.push_back(Token(TokenType::END_OF_EXPRESSION));

	//Parse
	Parser parser(tokens, *this);
	auto expression = parser.parse();
	EvalStack evalStack;

	//Evaluate
	expression->evaluate(*this, env, evalStack);

	if (evalStack.empty()) {
		throw std::runtime_error("Expected result.");
	}

	return evalStack.top();
}
