#include "calcengine.h"
#include "parser/parser.h"
#include "expressions/expression.h"
#include "core/operator.h"
#include "core/environment.h"
#include "visitors/calculationvisitor.h"
#include <stdexcept>
#include <queue>
#include <stack>
#include <cmath>

//Calc engine
CalcEngine::CalcEngine(std::ostream& os)
	: mEvalMode(ResultValueType::FLOAT) {
	mNumberTypes.emplace(ResultValueType::FLOAT, std::unique_ptr<FloatType>(new FloatType));
	mNumberTypes.emplace(ResultValueType::INTEGER, std::unique_ptr<IntegerType>(new IntegerType(os)));
	mNumberTypes.emplace(ResultValueType::COMPLEX, std::unique_ptr<ComplexType>(new ComplexType(os)));
}

NumberType& CalcEngine::currentNumberType() const {
	return *mNumberTypes.at(mEvalMode).get();
}

const BinaryOperators& CalcEngine::binaryOperators() const {
	return currentNumberType().binaryOperators();
}

bool CalcEngine::getBinaryOperator(OperatorChar opChar, Operator& op) const {
	auto operatorIterator = binaryOperators().find(opChar);
	if (operatorIterator != binaryOperators().end()) {
		op = operatorIterator->second;
		return true;
	}

	return false;
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

	//Evaluate
	CalculationVisitor calculationVisitor(*this, env);
	expression->accept(calculationVisitor, nullptr);

	auto& evalStack = calculationVisitor.evaluationStack();

	if (evalStack.empty()) {
		throw std::runtime_error("Expected result.");
	}

	return evalStack.top();
}
