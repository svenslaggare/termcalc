#include "expression.h"
#include "calcengine.h"
#include "environment.h"
#include "function.h"
#include <cmath>
#include <unordered_set>
#include <sstream>

//Double expression
DoubleExpression::DoubleExpression(double value): mValue(value) {

}

std::string DoubleExpression::toString() {
	return std::to_string(mValue);
}

void DoubleExpression::evaluate(Environment& env, EvalStack& evalStack) {
	evalStack.push(mValue);
}

//Long expression
Int64Expression::Int64Expression(std::int64_t value): mValue(value) {

}

std::string Int64Expression::toString() {
	return std::to_string(mValue);
}

void Int64Expression::evaluate(Environment& env, EvalStack& evalStack) {
	evalStack.push(mValue);
}

//Variable expression
VariableExpression::VariableExpression(std::string name)
	: mName(name) {

}

std::string VariableExpression::name() const {
	return mName;
}

std::string VariableExpression::toString() {
	return mName;
}

void VariableExpression::evaluate(Environment& env, EvalStack& evalStack) {
	ResultValue value;
	if (env.getVariable(mName, value, true)) {
		evalStack.push(value);
	} else {
		throw std::runtime_error("'" + mName + "' is not a defined variable. Type ':help' for a list of commands.");
	}
}

//Function call
FunctionCallExpression::FunctionCallExpression(std::string name, std::vector<std::unique_ptr<Expression>> arguments)
	: mName(name), mArguments(std::move(arguments)) {

}

std::string FunctionCallExpression::name() const {
	return mName;
}

std::size_t FunctionCallExpression::numArguments() const {
	return mArguments.size();
}

Expression* FunctionCallExpression::getArgument(std::size_t index) const {
	if (index < mArguments.size()) {
		return mArguments[index].get();
	} else {
		return nullptr;
	}
}

std::string FunctionCallExpression::toString() {
	std::ostringstream stream;

	stream << mName << "(";

	bool isFirst = true;

	for (auto& arg : mArguments) {
		if (!isFirst) {
			stream << ", ";
		}

		stream << arg->toString();

		isFirst = false;
	}

	stream << ")";

	return stream.str();
}

void FunctionCallExpression::evaluate(Environment& env, EvalStack& evalStack) {
	//Find the function
	if (env.functions().count(mName) == 0) {
		throw std::runtime_error("'" + mName + "' is not a defined function.");
	}

	auto& func = env.functions().at(mName);

	if (mArguments.size() != func.numArgs()) {
		throw std::runtime_error("Expected " + std::to_string(func.numArgs()) + " arguments but got " + std::to_string(mArguments.size()));
	}

	for (auto& arg : mArguments) {
		arg->evaluate(env, evalStack);
	}

	FnArgs args;
	for (std::size_t i = 0; i < func.numArgs(); i++) {
		auto arg = evalStack.top();
		evalStack.pop();
		args.insert(args.begin(), arg);
	}

	evalStack.push(func.apply(env, args));
}

//Binary operator expression
BinaryOperatorExpression::BinaryOperatorExpression(Operator op, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs)
	: mOp(op), mLHS(std::move(lhs)), mRHS(std::move(rhs)) {

}

std::string BinaryOperatorExpression::toString() {
	return mLHS->toString() + mOp.op().toString() + mRHS->toString();
}

void BinaryOperatorExpression::evaluate(Environment& env, EvalStack& evalStack) {
	if (mOp.op() == '=') {
		if (auto var = dynamic_cast<VariableExpression*>(mLHS.get())) {
			mRHS->evaluate(env, evalStack);

			auto value = evalStack.top().floatValue();
			evalStack.pop();

			env.set(var->name(), value);

			evalStack.push(value);
		} else if (auto func = dynamic_cast<FunctionCallExpression*>(mLHS.get())) {
			//Check that the parameters are variables
			std::unordered_set<std::string> usedParameterNames;
			std::vector<std::string> parameters;

			for (std::size_t i = 0; i < func->numArguments(); i++) {
				if (auto param = dynamic_cast<VariableExpression*>(func->getArgument(i))) {
					if (usedParameterNames.count(param->name()) == 0) {
						usedParameterNames.insert(param->name());
						parameters.push_back(param->name());
					} else {
						throw std::runtime_error("The parameter '" + param->name() + "' is already used.");
					}
				} else {
					throw std::runtime_error("Parameter number " + std::to_string(i) + " is not a variable.");
				}
			}

			env.define(Function(func->name(), parameters.size(), std::make_shared<FunctionBody>(parameters, std::move(mRHS))));
			evalStack.push(ResultValue((std::int64_t)0));
		} else {
			throw std::runtime_error("The left hand side must be a variable or a function definition.");
		}
	} else {
		mLHS->evaluate(env, evalStack);
		mRHS->evaluate(env, evalStack);

		auto op2 = evalStack.top();
		evalStack.pop();

		auto op1 = evalStack.top();
		evalStack.pop();

		evalStack.push(mOp.apply(env.evalMode(), op1, op2));
	}
}

//Unary expression
UnaryOperatorExpression::UnaryOperatorExpression(Operator op, std::unique_ptr<Expression> operand)
	: mOp(op), mOperand(std::move(operand)) {

}

std::string UnaryOperatorExpression::toString() {
	return mOp.op().toString() + mOperand->toString();
}

void UnaryOperatorExpression::evaluate(Environment& env, EvalStack& evalStack) {
	mOperand->evaluate(env, evalStack);

	auto operand = evalStack.top();
	evalStack.pop();

	evalStack.push(mOp.apply(env.evalMode(), operand));
}