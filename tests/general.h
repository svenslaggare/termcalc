#include <string>
#include <vector>
#include <cxxtest/TestSuite.h>
#include "../src/calcengine.h"
#include "../src/environment.h"
#include "../src/parser.h"
#include "../src/numbertype.h"

using Tokens = std::vector<Token>;

class TermCalcTestSuite : public CxxTest::TestSuite {
public:
    void testTokenizer() {
		FloatType floatType;
		IntegerType integerType;

        TS_ASSERT_EQUALS(
            Tokenizer::tokenize("2.0+5", floatType),
            Tokens({ Token(2.0), Token(TokenType::OPERATOR, '+'), Token(5.0) }));

        TS_ASSERT_EQUALS(
            Tokenizer::tokenize("(2.0+5) * 7.0", floatType),
            Tokens({ Token(TokenType::LEFT_PARENTHESIS),
                     Token(2.0), Token(TokenType::OPERATOR, '+'), Token(5.0),
                     Token(TokenType::RIGHT_PARENTHESIS),
                     Token(TokenType::OPERATOR, '*'), Token( 7.0) }));

        TS_ASSERT_EQUALS(
            Tokenizer::tokenize("1 << 10", floatType),
            Tokens({ Token(1L), Token(TokenType::TWO_CHAR_OPERATOR, '<', '<'), Token(10L) }));
    }

    void testEvalVariables() {
        CalcEngine engine;
        TS_ASSERT_DELTA(engine.eval("2 * pi").floatValue(), ResultValue(6.28).floatValue(), 0.01);

        Environment env;
        env.set("e", 2.718281828);
        TS_ASSERT_DELTA(engine.eval("e^2", env).floatValue(), ResultValue(7.38905609893).floatValue(), 0.01);

        TS_ASSERT_THROWS(engine.eval("2 * x").floatValue(), std::runtime_error);
    }

    void testDefineFunctions() {
        CalcEngine engine;
        Environment env;
        engine.eval("f(x)=x^2", env);
        TS_ASSERT_EQUALS(engine.eval("f(4)", env), ResultValue(16.0));

        engine.setEvalMode(ResultValueType::INTEGER);
        TS_ASSERT_EQUALS(engine.eval("f(4)", env), ResultValue(16L));
    }

    void testInvalidEval() {
        CalcEngine engine;
        TS_ASSERT_THROWS(engine.eval("3 ++ 2"), std::runtime_error);
        TS_ASSERT_THROWS(engine.eval("3+"), std::runtime_error);
    }

    void testAssignment() {
        CalcEngine engine;
        Environment env;
        TS_ASSERT_EQUALS(engine.eval("x = 2 * 5", env), ResultValue(10.0));
        TS_ASSERT_EQUALS(env.valueOf("x"), ResultValue(10.0));
    }

    void testConvert() {
        CalcEngine engine;
		Environment environment;
        engine.eval("x=3.14", environment);
        engine.setEvalMode(ResultValueType::INTEGER);

        TS_ASSERT_EQUALS(engine.eval("x+2", environment), ResultValue(5L));
    }
};
