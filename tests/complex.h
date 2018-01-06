#include <string>
#include <vector>
#include <cxxtest/TestSuite.h>
#include "../src/calculation/calcengine.h"
#include "../src/calculation/environment.h"
#include "../src/parser/parser.h"
#include "../src/calculation/numbertypes/numbertype.h"
#include "../src/core/resultvalue.h"

using Tokens = std::vector<Token>;

class ComplexTestSuite : public CxxTest::TestSuite {
public:
	void testTokenize() {
		ComplexType complexType;

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("2i", complexType),
			Tokens({ Token(NumericConstant({ NumericConstantChar::Two, NumericConstantChar::ImaginaryUnit })) }));

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("i", complexType),
			Tokens({ Token(NumericConstant({ NumericConstantChar::ImaginaryUnit })) }));

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("10", complexType),
			Tokens({ Token(NumericConstant({ NumericConstantChar::One, NumericConstantChar::Zero })) }));

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("0", complexType),
			Tokens({ Token(NumericConstant({ NumericConstantChar::Zero })) }));
	}

	void testEval() {
		CalculationEngine engine;
		engine.setEvalMode(ResultValueType::COMPLEX);
		TS_ASSERT_EQUALS(engine.evaluate("i"), ResultValue(Complex(0, 1)));
		TS_ASSERT_EQUALS(engine.evaluate("3i"), ResultValue(Complex(0, 3)));
		TS_ASSERT_EQUALS(engine.evaluate("1+3i"), ResultValue(Complex(1, 3)));
		TS_ASSERT_EQUALS(engine.evaluate("(2+3i)*(2+6i)"), ResultValue(Complex(-14, 18)));
		TS_ASSERT_EQUALS(engine.evaluate("2-i"), ResultValue(Complex(2, -1)));
		TS_ASSERT_EQUALS(engine.evaluate("-2i"), ResultValue(Complex(0, -2)));
	}

	void testEvalVariables() {
		CalculationEngine engine;
		engine.setEvalMode(ResultValueType::COMPLEX);
		TS_ASSERT_EQUALS(engine.evaluate("e^(pi*i)"), ResultValue(Complex(-1, 0)));
	}

	void testEvalFunctions() {
		CalculationEngine engine;
		engine.setEvalMode(ResultValueType::COMPLEX);
		TS_ASSERT_EQUALS(engine.evaluate("sin(-2i)"), ResultValue(std::sin(Complex(0, -2))));

		TS_ASSERT_EQUALS(engine.evaluate("real(5-2i)"), ResultValue(Complex(5, 0)));
		TS_ASSERT_EQUALS(engine.evaluate("imag(5-2i)"), ResultValue(Complex(-2, 0)));
		TS_ASSERT_EQUALS(engine.evaluate("arg(5-2i)"), ResultValue(Complex(std::arg(Complex(5, -2)), 0)));
		TS_ASSERT_EQUALS(engine.evaluate("abs(5-2i)"), ResultValue(Complex(std::abs(Complex(5, -2)), 0)));
		TS_ASSERT_EQUALS(engine.evaluate("polar(10, 5.12)"), ResultValue(std::polar(10.0, 5.12)));
		TS_ASSERT_EQUALS(engine.evaluate("conj(5-2i)"), ResultValue(Complex(5, 2)));
	}

	void testToString() {
		TS_ASSERT_EQUALS("2", ResultValue(Complex(2, 0)).toString());
		TS_ASSERT_EQUALS("-2", ResultValue(Complex(-2, 0)).toString());
		TS_ASSERT_EQUALS("2+i", ResultValue(Complex(2, 1)).toString());
		TS_ASSERT_EQUALS("i", ResultValue(Complex(0, 1)).toString());
		TS_ASSERT_EQUALS("-i", ResultValue(Complex(0, -1)).toString());
		TS_ASSERT_EQUALS("2-7i", ResultValue(Complex(2, -7)).toString());
		TS_ASSERT_EQUALS("-2-7i", ResultValue(Complex(-2, -7)).toString());
	}
};