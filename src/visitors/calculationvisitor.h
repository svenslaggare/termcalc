#pragma once
#include "dfsvisitor.h"

/**
 * Represents a calculation visitor
 */
class CalculationVisitor : public DfsVisitor {
private:
	CalcEngine& mCalcEngine;
	EvalStack mEvaluationStack;
	Environment& mEnvironment;
public:
	/**
	 * Creates a new calculation visitor
	 * @param calcEngine The calculation engine
	 * @param environment The environment
	 */
	CalculationVisitor(CalcEngine& calcEngine, Environment& environment);

	/**
	 * Returns the evaluation stack
	 */
	const EvalStack& evaluationStack() const;

	void visit(Expression* parent, NumberExpression* expression) override;
	void visit(Expression* parent, VariableExpression* expression) override;
	void visit(Expression* parent, FunctionCallExpression* expression) override;
	void visit(Expression* parent, BinaryOperatorExpression* expression) override;
	void visit(Expression* parent, UnaryOperatorExpression* expression) override;
};