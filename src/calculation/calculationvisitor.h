#pragma once
#include "../visitors/dfsvisitor.h"

using EvaluationStack = std::stack<ResultValue>;

/**
 * Represents a calculation visitor
 */
class CalculationVisitor : public DfsVisitor {
private:
	CalculationEngine& mCalcEngine;
	EvaluationStack mEvaluationStack;
	Environment& mEnvironment;

	/**
	 * Pops the from the evaluation stack
	 */
	ResultValue popStack();

	/**
	 * Evaluates the given user defined function
	 * @param userFunction The body of the function
	 * @param args The arguments
	 */
	ResultValue evaluateUserFunction(UserFunction* userFunction, const FunctionArguments& args);

	/**
	 * Defines a variable
	 * @param expression The expression
	 * @param variable The variable name
	 */
	void defineVariable(BinaryOperatorExpression* expression, VariableExpression* variable);

	/**
	 * Defines an user function
	 * @param expression The expression
	 * @param func The function prototype
	 */
	void defineUserFunction(BinaryOperatorExpression* expression, FunctionCallExpression* func);
public:
	/**
	 * Creates a new calculation visitor
	 * @param calcEngine The calculation engine
	 * @param environment The environment
	 */
	CalculationVisitor(CalculationEngine& calcEngine, Environment& environment);

	/**
	 * Returns the evaluation stack
	 */
	const EvaluationStack& evaluationStack() const;

	void visit(Expression* parent, NumberExpression* expression) override;
	void visit(Expression* parent, VariableExpression* expression) override;
	void visit(Expression* parent, FunctionCallExpression* expression) override;
	void visit(Expression* parent, BinaryOperatorExpression* expression) override;
	void visit(Expression* parent, UnaryOperatorExpression* expression) override;
};