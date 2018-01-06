#include <string>
#include <vector>
#include <cxxtest/TestSuite.h>
#include "../src/calculator.h"

class CalculatorTestSuite : public CxxTest::TestSuite {
public:
	void testPrintInBase() {
		std::stringstream stream;
		Calculator calculator(stream);
		calculator.setEvalMode(ResultValueType::INTEGER);

		calculator.setPrintNumBase(16);
		calculator.execute("4251");
		TS_ASSERT_EQUALS(stream.str(), "0x109b\n");

		stream.str("");
		calculator.setPrintNumBase(2);
		calculator.execute("4251");
		TS_ASSERT_EQUALS(stream.str(), "0b1000010011011\n");
	}

	void testToLargeForInt() {
		std::stringstream stream;
		Calculator calculator(stream);
		calculator.setEvalMode(ResultValueType::INTEGER);

		calculator.execute("54645345364563453534654646");
		TS_ASSERT_EQUALS(stream.str(), "Error: The given number is too large.\n");
	}

	void testPrintInPolar() {
		std::stringstream stream;
		Calculator calculator(stream);
		calculator.setEvalMode(ResultValueType::COMPLEX);
		calculator.setPrintInPolar(true);

		calculator.execute("2+4i");
		TS_ASSERT_EQUALS(stream.str(), "4.47214 * e^(1.10715i)\n");
	}
};
