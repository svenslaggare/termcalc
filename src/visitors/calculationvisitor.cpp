#include "calculationvisitor.h"
#include "../calcengine.h"
#include "../core/environment.h"

#include <unordered_set>

CalculationVisitor::CalculationVisitor(CalcEngine& calcEngine, Environment& environment)
	: mCalcEngine(calcEngine), mEnvironment(environment) {

}

const EvaluationStack& CalculationVisitor::evaluationStack() const {
	return mEvaluationStack;
}

ResultValue CalculationVisitor::popStack() {
	auto value = mEvaluationStack.top();
	mEvaluationStack.pop();
	return value;
}

void CalculationVisitor::visit(Expression* parent, NumberExpression* expression) {
	mEvaluationStack.push(mCalcEngine.currentNumberType().toResultValue(expression->value()));
}

void CalculationVisitor::visit(Expression* parent, VariableExpression* expression) {
	ResultValue value;
	if (mEnvironment.getVariable(expression->name(), value, true)) {
		mEvaluationStack.push(value);
	} else {
		throw std::runtime_error("'" + expression->name() + "' is not a defined variable. Type ':help' for a list of commands.");
	}
}

ResultValue CalculationVisitor::evaluateUserFunction(UserFunction* userFunction, const FunctionArguments& args) {
	//First, save the environment values
	std::unordered_map<std::string, ResultValue> savedValues;
	for (auto parameter : userFunction->parameters()) {
		if (mEnvironment.variables().count(parameter) > 0) {
			savedValues.insert({ parameter, mEnvironment.variables().at(parameter) });
		}
	}

	//Set the parameter values
	std::size_t i = 0;
	for (auto parameter : userFunction->parameters()) {
		mEnvironment.set(parameter, args[i]);
		i++;
	}

	//Apply the function
	userFunction->body()->accept(*this, nullptr);

	if (mEvaluationStack.size() != 1) {
		throw std::runtime_error("Expected result.");
	}

	//Restore environment values
	for (auto parameter : userFunction->parameters()) {
		mEnvironment.unset(parameter);

		if (savedValues.count(parameter) > 0) {
			mEnvironment.set(parameter, savedValues[parameter]);
		}
	}

	return popStack();
}

void CalculationVisitor::visit(Expression* parent, FunctionCallExpression* expression) {
	//Find the function
	auto& func = mEnvironment.getFunction(expression->name(), expression->numArguments());

	if (expression->numArguments() != func.numArgs()) {
		throw std::runtime_error(
			"Expected " + std::to_string(func.numArgs())
			+ " arguments but got " + std::to_string(expression->numArguments()) + ".");
	}

	DfsVisitor::visit(parent, expression);

	FunctionArguments args;
	for (std::size_t i = 0; i < func.numArgs(); i++) {
		args.insert(args.begin(), popStack());
	}

	if (!func.isUserDefined()) {
		mEvaluationStack.push(func.externalFunction()(args));
	} else {
		mEvaluationStack.push(evaluateUserFunction(func.userFunction().get(), args));
	}
}

void CalculationVisitor::defineVariable(BinaryOperatorExpression* expression, VariableExpression* variable) {
	expression->rightHandSide()->accept(*this, expression);

	auto value = popStack();
	mEnvironment.set(variable->name(), value);
	mEvaluationStack.push(value);
}

void CalculationVisitor::defineUserFunction(BinaryOperatorExpression* expression, FunctionCallExpression* func) {
	//Check that the parameters are variables
	std::unordered_set<std::string> usedParameterNames;
	std::vector<std::string> parameters;

	for (std::size_t i = 0; i < func->numArguments(); i++) {
		if (auto parameter = dynamic_cast<VariableExpression*>(func->getArgument(i))) {
			if (usedParameterNames.count(parameter->name()) == 0) {
				usedParameterNames.insert(parameter->name());
				parameters.push_back(parameter->name());
			} else {
				throw std::runtime_error("The parameter '" + parameter->name() + "' is already used.");
			}
		} else {
			throw std::runtime_error("Parameter number " + std::to_string(i) + " is not a variable.");
		}
	}

	auto rhs = expression->releaseRightHandSide();
	mEnvironment.define(Function(
		func->name(),
		parameters.size(),
		std::make_shared<UserFunction>(parameters, std::unique_ptr<Expression>(rhs))));
	mEvaluationStack.push(ResultValue());
}

void CalculationVisitor::visit(Expression* parent, BinaryOperatorExpression* expression) {
	if (expression->op() == '=') {
		if (auto var = dynamic_cast<VariableExpression*>(expression->leftHandSide())) {
			defineVariable(expression, var);
		} else if (auto func = dynamic_cast<FunctionCallExpression*>(expression->leftHandSide())) {
			defineUserFunction(expression, func);
		} else {
			throw std::runtime_error("The left hand side must be a variable or a function definition.");
		}
	} else {
		//It's possible that the expression was defined using a type that don't support the current operator.
		if (mCalcEngine.binaryOperators().count(expression->op()) == 0) {
			throw std::runtime_error("The current mode does not support the operator '" + expression->op().toString() + "'.");
		}

		DfsVisitor::visit(parent, expression);

		auto op2 = popStack();
		auto op1 = popStack();

		auto& op = mCalcEngine.binaryOperators().at(expression->op());
		mEvaluationStack.push(op.apply(op1, op2));
	}
}

void CalculationVisitor::visit(Expression* parent, UnaryOperatorExpression* expression) {
	//It's possible that the expression was defined using a type that don't support the current operator.
	if (mCalcEngine.unaryOperators().count(expression->op()) == 0) {
		throw std::runtime_error("The current mode does not support the operator '" + expression->op().toString() + "'.");
	}

	DfsVisitor::visit(parent, expression);

	auto operand = popStack();

	auto& op = mCalcEngine.unaryOperators().at(expression->op());
	mEvaluationStack.push(op.apply(operand));
}
