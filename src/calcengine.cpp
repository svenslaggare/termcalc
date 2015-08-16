#include "calcengine.h"
#include "parser.h"
#include "expression.h"
#include "operator.h"
#include "environment.h"
#include <stdexcept>
#include <queue>
#include <stack>

//Calc engine
CalcEngine::CalcEngine()
	: mEvalMode(ResultValueType::FLOAT) {
	mBinaryOperators = { {
		{ '^', Operator('^', 6, OperatorAssociativity::RIGHT) },
		{ '*', Operator('*', 5, OperatorAssociativity::LEFT) },
		{ '/', Operator('/', 5, OperatorAssociativity::LEFT) },
		{ '%', Operator('%', 5, OperatorAssociativity::LEFT) },
		{ '+', Operator('+', 4, OperatorAssociativity::LEFT) },
		{ '-', Operator('-', 4, OperatorAssociativity::LEFT) },
		{ OperatorChar('<', '<'), Operator(OperatorChar('<', '<'), 3, OperatorAssociativity::LEFT) },
		{ OperatorChar('>', '>'), Operator(OperatorChar('>', '>'), 3, OperatorAssociativity::LEFT) },
		{ '|', Operator('|', 2, OperatorAssociativity::LEFT) },
		{ '&', Operator('&', 2, OperatorAssociativity::LEFT) },
		{ '=', Operator('=', 1, OperatorAssociativity::RIGHT) }
	}, 15, OperatorChar::HASH, OperatorChar::EQUAL };

	mUnaryOperators = {
		{ '-', Operator('-', 7, OperatorAssociativity::LEFT, true) },
		{ '~', Operator('~', 7, OperatorAssociativity::LEFT, true) },
	};
}

const CalcEngine::BinaryOperators& CalcEngine::binaryOperators() const {
	return mBinaryOperators;
}

const std::unordered_map<char, Operator>& CalcEngine::unaryOperators() const {
	return mUnaryOperators;
}

ResultValueType CalcEngine::evalMode() const {
	return mEvalMode;
}

void CalcEngine::setEvalMode(ResultValueType evalMode) {
	mEvalMode = evalMode;
}

ResultValue CalcEngine::eval(std::string expressionString) {
	Environment env;
	env.setEvalMode(mEvalMode);
	return eval(expressionString, env);
}

ResultValue CalcEngine::eval(std::string expressionString, Environment& env) {
	env.setEvalMode(mEvalMode);

	//Tokenize
	auto tokens = Tokenizer::tokenize(expressionString);
	tokens.push_back(Token(TokenType::END_OF_EXPRESSION));

	//Parse
	Parser parser(tokens, *this);
	auto expression = parser.parse();
	EvalStack evalStack;

	//Evaluate
	expression->evaluate(env, evalStack);

	if (evalStack.empty()) {
		throw std::runtime_error("Expected result.");
	}

	return evalStack.top();
}
