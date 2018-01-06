#include <string>
#include <vector>
#include <cxxtest/TestSuite.h>
#include "../src/calculation/calculationengine.h"
#include "../src/calculation/environment.h"
#include "../src/parser/parser.h"
#include "../src/calculation/numbertypes/numbertype.h"

using Tokens = std::vector<Token>;

class TermCalcTestSuite : public CxxTest::TestSuite {
public:
    void testTokenizer() {
		FloatType floatType;
		IntegerType integerType;

        TS_ASSERT_EQUALS(
            Tokenizer::tokenize("2.0+5.0", floatType),
            Tokens({ Token(2.0), Token(TokenType::OPERATOR, '+'), Token(5.0) }));

        TS_ASSERT_EQUALS(
            Tokenizer::tokenize("(2.0+5.0) * 7.0", floatType),
            Tokens({ Token(TokenType::LEFT_PARENTHESIS),
                     Token(2.0), Token(TokenType::OPERATOR, '+'), Token(5.0),
                     Token(TokenType::RIGHT_PARENTHESIS),
                     Token(TokenType::OPERATOR, '*'), Token(7.0) }));

        TS_ASSERT_EQUALS(
            Tokenizer::tokenize("1 << 10", floatType),
            Tokens({ Token((std::int64_t)1LL), Token(TokenType::TWO_CHAR_OPERATOR, '<', '<'), Token((std::int64_t)10LL) }));
    }

    void testEvalVariables() {
        CalculationEngine engine;
        TS_ASSERT_DELTA(engine.evaluate("2 * pi").floatValue(), ResultValue(6.28).floatValue(), 0.01);

        Environment env;
        env.set("e", 2.718281828);
        TS_ASSERT_DELTA(engine.evaluate("e^2", env).floatValue(), ResultValue(7.38905609893).floatValue(), 0.01);

        TS_ASSERT_THROWS(engine.evaluate("2 * x").floatValue(), std::runtime_error);
    }

    void testDefineFunctions() {
        CalculationEngine engine;
        Environment env;
		engine.evaluate("f(x)=x^2", env);
        TS_ASSERT_EQUALS(engine.evaluate("f(4)", env), ResultValue(16.0));

        engine.setEvalMode(ResultValueType::INTEGER);
        TS_ASSERT_EQUALS(engine.evaluate("f(4)", env), ResultValue((std::int64_t)16LL));
    }

	void testDefineFunctionsOverload() {
		CalculationEngine engine;
		Environment env;
		engine.evaluate("f(x)=x^2", env);
		engine.evaluate("f(x, y)=x / y", env);

		TS_ASSERT_EQUALS(engine.evaluate("f(4)", env), ResultValue(16.0));
		TS_ASSERT_EQUALS(engine.evaluate("f(2, 4)", env), ResultValue(2.0 / 4));
	}

    void testInvalidEval() {
        CalculationEngine engine;
        TS_ASSERT_THROWS(engine.evaluate("3 ++ 2"), std::runtime_error);
        TS_ASSERT_THROWS(engine.evaluate("3+"), std::runtime_error);
    }

    void testAssignment() {
        CalculationEngine engine;
        Environment env;
        TS_ASSERT_EQUALS(engine.evaluate("x = 2 * 5", env), ResultValue(10.0));
        TS_ASSERT_EQUALS(env.valueOf("x"), ResultValue(10.0));
    }

    void testConvert() {
        CalculationEngine engine;
		Environment environment;
		engine.evaluate("x=3.14", environment);
        engine.setEvalMode(ResultValueType::INTEGER);

        TS_ASSERT_EQUALS(engine.evaluate("x+2", environment), ResultValue((std::int64_t)5LL));
    }
};
