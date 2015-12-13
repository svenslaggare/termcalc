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
	mBinaryOperators = { {
		{ '^', Operator('^', 6, OperatorAssociativity::RIGHT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			return binaryOperator(NumberOperators::Pow, evalMode, lhs, rhs);
		}) },
		{ '*', Operator('*', 5, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			return binaryOperator(NumberOperators::Mul, evalMode, lhs, rhs);
		}) },
		{ '/', Operator('/', 5, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			return binaryOperator(NumberOperators::Div, evalMode, lhs, rhs);
		}) },
		{ '%', Operator('%', 5, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			return binaryOperator(NumberOperators::Rem, evalMode, lhs, rhs);
		}) },
		{ '+', Operator('+', 4, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			return binaryOperator(NumberOperators::Add, evalMode, lhs, rhs);
		}) },
		{ '-', Operator('-', 4, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			return binaryOperator(NumberOperators::Sub, evalMode, lhs, rhs);
		}) },
		{ OperatorChar('<', '<'), Operator(OperatorChar('<', '<'), 3, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			return binaryOperator(NumberOperators::LeftShift, evalMode, lhs, rhs);
		}) },
		{ OperatorChar('>', '>'), Operator(OperatorChar('>', '>'), 3, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			return binaryOperator(NumberOperators::RightShift, evalMode, lhs, rhs);
		}) },
		{ '|', Operator('|', 2, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			return binaryOperator(NumberOperators::BitwiseOr, evalMode, lhs, rhs);
		}) },
		{ '&', Operator('&', 2, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			return binaryOperator(NumberOperators::BitwiseAnd, evalMode, lhs, rhs);
		}) },
		{ '=', Operator('=', 1, OperatorAssociativity::RIGHT, [&](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			return ResultValue();
		}) }
	} };

	mUnaryOperators = {
		{ '-', Operator('-', 7, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue op) {
			return unaryOperator(NumberOperators::Neg, evalMode, op);
		}) },
		{ '~', Operator('~', 7, OperatorAssociativity::LEFT, [&](ResultValueType evalMode, ResultValue op) {
			return unaryOperator(NumberOperators::Inv, evalMode, op);
		}) },
	};

	mNumberTypes.emplace_back(std::unique_ptr<FloatType>(new FloatType));
	mNumberTypes.emplace_back(std::unique_ptr<IntegerType>(new IntegerType));
}

ResultValue CalcEngine::binaryOperator(NumberOperators op, ResultValueType evalMode, ResultValue x, ResultValue y) {
	return mNumberTypes.at((std::size_t)evalMode)->binaryOperator(op, x, y);
}

ResultValue CalcEngine::unaryOperator(NumberOperators op, ResultValueType evalMode, ResultValue x) {
	return mNumberTypes.at((std::size_t)evalMode)->unaryOperator(op, x);
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
