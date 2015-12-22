#include <string>
#include <vector>
#include <cxxtest/TestSuite.h>
#include "../src/calcengine.h"
#include "../src/environment.h"
#include "../src/parser.h"
#include "../src/numbertype.h"
#include "../src/resultvalue.h"

using Tokens = std::vector<Token>;

class ComplexTestSuite : public CxxTest::TestSuite {
public:
	void testTokenize() {
		ComplexType complexType;

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("2i", complexType),
			Tokens({ Token(Complex(0, 2)) }));

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("i", complexType),
			Tokens({ Token(Complex(0, 1)) }));

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("10", complexType),
			Tokens({ Token(Complex(10, 0)) }));

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("0i", complexType),
			Tokens({ Token(Complex(0, 0)) }));

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("0", complexType),
			Tokens({ Token(Complex(0, 0)) }));
	}

	void testEval() {
		CalcEngine engine;
		engine.setEvalMode(ResultValueType::COMPLEX);
		TS_ASSERT_EQUALS(engine.eval("1+3i"), ResultValue(Complex(1, 3)));
		TS_ASSERT_EQUALS(engine.eval("(2+3i)*(2+6i)"), ResultValue(Complex(-14, 18)));
	}

	void testEvalVariables() {
		CalcEngine engine;
		engine.setEvalMode(ResultValueType::COMPLEX);
		TS_ASSERT_EQUALS(engine.eval("e^(pi*i)"), ResultValue(Complex(-1, 0)));
	}

	void testEvalFunctions() {
		CalcEngine engine;
		engine.setEvalMode(ResultValueType::COMPLEX);
		TS_ASSERT_EQUALS(engine.eval("sin(-2i)"), ResultValue(std::sin(Complex(0, -2))));
	}
};