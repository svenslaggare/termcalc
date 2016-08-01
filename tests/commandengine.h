#include <string>
#include <vector>
#include <cxxtest/TestSuite.h>
#include "../src/commandengine.h"

class CommandEngineTestSuite : public CxxTest::TestSuite {
public:
	void testPrintInBase() {
		std::stringstream stream;
		CommandEngine commandEngine(stream);
		commandEngine.setEvalMode(ResultValueType::INTEGER);

		commandEngine.setPrintNumBase(16);
		commandEngine.execute("4251");
		TS_ASSERT_EQUALS(stream.str(), "0x109b\n");

		stream.str("");
		commandEngine.setPrintNumBase(2);
		commandEngine.execute("4251");
		TS_ASSERT_EQUALS(stream.str(), "0b1000010011011\n");
	}

	void testToLargeForInt() {
		std::stringstream stream;
		CommandEngine commandEngine(stream);
		commandEngine.setEvalMode(ResultValueType::INTEGER);

		commandEngine.execute("54645345364563453534654646");
		TS_ASSERT_EQUALS(stream.str(), "Error: The given number is too large.\n");
	}

	void testPrintInPolar() {
		std::stringstream stream;
		CommandEngine commandEngine(stream);
		commandEngine.setEvalMode(ResultValueType::COMPLEX);
		commandEngine.setPrintInPolar(true);

		commandEngine.execute("2+4i");
		TS_ASSERT_EQUALS(stream.str(), "4.47214 * e^(1.10715i)\n");
	}
};
