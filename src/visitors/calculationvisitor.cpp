#include "calculationvisitor.h"
#include "../calcengine.h"
#include "../core/environment.h"

#include <unordered_set>

CalculationVisitor::CalculationVisitor(CalcEngine& calcEngine, Environment& environment)
	: mCalcEngine(calcEngine), mEnvironment(environment) {

}

const EvalStack& CalculationVisitor::evaluationStack() const {
	return mEvaluationStack;
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

void CalculationVisitor::visit(Expression* parent, FunctionCallExpression* expression) {
	//Find the function
	auto& func = mEnvironment.getFunction(expression->name(), expression->numArguments());

	if (expression->numArguments() != func.numArgs()) {
		throw std::runtime_error(
			"Expected " + std::to_string(func.numArgs())
			+ " arguments but got " + std::to_string(expression->numArguments()) + ".");
	}

	DfsVisitor::visit(parent, expression);

	FnArgs args;
	for (std::size_t i = 0; i < func.numArgs(); i++) {
		auto arg = mEvaluationStack.top();
		mEvaluationStack.pop();
		args.insert(args.begin(), arg);
	}

	mEvaluationStack.push(func.apply(mCalcEngine, mEnvironment, args));
}

void CalculationVisitor::visit(Expression* parent, BinaryOperatorExpression* expression) {
	if (expression->op() == '=') {
		if (auto var = dynamic_cast<VariableExpression*>(expression->leftHandSide())) {
			expression->rightHandSide()->accept(*this, expression);

			auto value = mEvaluationStack.top();
			mEvaluationStack.pop();

			mEnvironment.set(var->name(), value);
			mEvaluationStack.push(value);
		} else if (auto func = dynamic_cast<FunctionCallExpression*>(expression->leftHandSide())) {
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

			auto rhs = expression->releaseRightHandSide();
			mEnvironment.define(Function(
				func->name(),
				parameters.size(),
				std::make_shared<FunctionBody>(parameters, std::unique_ptr<Expression>(rhs))));
			mEvaluationStack.push(ResultValue());
		} else {
			throw std::runtime_error("The left hand side must be a variable or a function definition.");
		}
	} else {
		//It's possible that the expression was defined using a type that don't support the current operator.
		if (mCalcEngine.binaryOperators().count(expression->op()) == 0) {
			throw std::runtime_error("The current mode does not support the operator '" + expression->op().toString() + "'.");
		}

		DfsVisitor::visit(parent, expression);

		auto op2 = mEvaluationStack.top();
		mEvaluationStack.pop();

		auto op1 = mEvaluationStack.top();
		mEvaluationStack.pop();

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

	auto operand = mEvaluationStack.top();
	mEvaluationStack.pop();

	auto& op = mCalcEngine.unaryOperators().at(expression->op());
	mEvaluationStack.push(op.apply(operand));
}
