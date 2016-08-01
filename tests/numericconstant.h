#include <string>
#include <vector>
#include <cxxtest/TestSuite.h>
#include "../src/numericconstant.h"

class NumericConstantTestSuite : public CxxTest::TestSuite {
public:
	void testString() {
		TS_ASSERT_EQUALS(
			NumericConstant("-135").chars(),
			std::vector<NumericConstantChar>({
				 NumericConstantChar::MinusSign,
				 NumericConstantChar::One,
				 NumericConstantChar::Three,
				 NumericConstantChar::Five
			}));

		TS_ASSERT_EQUALS(
			NumericConstant("135").chars(),
			std::vector<NumericConstantChar>({
				 NumericConstantChar::One, NumericConstantChar::Three, NumericConstantChar::Five
			 }));

		TS_ASSERT_EQUALS(
			NumericConstant("135.126").chars(),
			std::vector<NumericConstantChar>({
				NumericConstantChar::One, NumericConstantChar::Three, NumericConstantChar::Five,
				NumericConstantChar::DecimalPoint, NumericConstantChar::One, NumericConstantChar::Two, NumericConstantChar::Six,
			}));

		TS_ASSERT_EQUALS(
			NumericConstant("-135.126").chars(),
			std::vector<NumericConstantChar>({
				 NumericConstantChar::MinusSign, NumericConstantChar::One, NumericConstantChar::Three, NumericConstantChar::Five,
				 NumericConstantChar::DecimalPoint, NumericConstantChar::One, NumericConstantChar::Two, NumericConstantChar::Six,
			}));

		TS_ASSERT_EQUALS(
			NumericConstant("135i").chars(),
			std::vector<NumericConstantChar>({
				 NumericConstantChar::One,
				 NumericConstantChar::Three,
				 NumericConstantChar::Five,
				 NumericConstantChar::ImaginaryUnit
			}));
	}

	void testInt() {
		TS_ASSERT_EQUALS(
			NumericConstant((std::int64_t)-135LL).chars(),
			std::vector<NumericConstantChar>({
				 NumericConstantChar::MinusSign,
				 NumericConstantChar::One,
				 NumericConstantChar::Three,
				 NumericConstantChar::Five
			 }));

		TS_ASSERT_EQUALS(
			NumericConstant((std::int64_t)135LL).chars(),
			std::vector<NumericConstantChar>({
				NumericConstantChar::One, NumericConstantChar::Three, NumericConstantChar::Five
			}));
	}

	void testFloat() {
		TS_ASSERT_EQUALS(
			NumericConstant(135.126).chars(),
			std::vector<NumericConstantChar>({
				 NumericConstantChar::One, NumericConstantChar::Three, NumericConstantChar::Five,
				 NumericConstantChar::DecimalPoint,
				 NumericConstantChar::One, NumericConstantChar::Two, NumericConstantChar::Six,
				 NumericConstantChar::Zero, NumericConstantChar::Zero, NumericConstantChar::Zero
			}));

		TS_ASSERT_EQUALS(
			NumericConstant(-135.126).chars(),
			std::vector<NumericConstantChar>({
				 NumericConstantChar::MinusSign,
				 NumericConstantChar::One, NumericConstantChar::Three, NumericConstantChar::Five,
				 NumericConstantChar::DecimalPoint,
				 NumericConstantChar::One, NumericConstantChar::Two, NumericConstantChar::Six,
				 NumericConstantChar::Zero, NumericConstantChar::Zero, NumericConstantChar::Zero
			}));
	}
};
