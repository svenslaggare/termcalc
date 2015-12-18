#include "numbertype.h"
#include <cmath>

//Integer type
namespace {
	//Parses a 64 bits integer in the given base
	std::int64_t parseInt64(std::string str, int base) {
		#if defined(__MINGW32__)
		return std::stoll(str, nullptr, base);
		#else
		return std::stol(str, nullptr, base);
		#endif
	}
}
IntegerType::IntegerType() {
	mBinaryOperators = {
		 { '^', Operator('^', 6, OperatorAssociativity::RIGHT, [&](ResultValue lhs, ResultValue rhs) {
			 return power(lhs.intValue(), rhs.intValue());
		 }) },
		 { '*', Operator('*', 5, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() * rhs.intValue();
		 }) },
		 { '/', Operator('/', 5, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() / rhs.intValue();
		 }) },
		 { '%', Operator('%', 5, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() % rhs.intValue();
		 }) },
		 { '+', Operator('+', 4, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() + rhs.intValue();
		 }) },
		 { '-', Operator('-', 4, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() - rhs.intValue();
		 }) },
		 { OperatorChar('<', '<'), Operator(OperatorChar('<', '<'), 3, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() << rhs.intValue();
		 }) },
		 { OperatorChar('>', '>'), Operator(OperatorChar('>', '>'), 3, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() >> rhs.intValue();
		 }) },
		 { '|', Operator('|', 2, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() | rhs.intValue();
		 }) },
		 { '&', Operator('&', 2, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() & rhs.intValue();
		 }) },
		 { '=', Operator('=', 1, OperatorAssociativity::RIGHT, [&](ResultValue lhs, ResultValue rhs) {
			 return ResultValue();
		 }) }
	 };

	mUnaryOperators = {
		{ '-', Operator('-', 7, OperatorAssociativity::LEFT, [&](ResultValue op) {
			return -op.intValue();
		}) },
		{ '~', Operator('~', 7, OperatorAssociativity::LEFT, [&](ResultValue op) {
			return ~op.intValue();
		}) },
	};
}

std::int64_t IntegerType::power(std::int64_t x, std::int64_t n) const {
	//Using the squaring algorithm
	if (n == 0) {
		return 1;
	}

	std::int64_t y = 1;

	while (n > 1) {
		if (n % 2 == 0) {
			x *= x;
			n /= 2;
		} else {
			y *= x;
			x *= x;
			n = (n - 1) / 2;
		}
	}

	return x * y;
}

const BinaryOperators& IntegerType::binaryOperators() const {
	return mBinaryOperators;
}

const UnaryOperators& IntegerType::unaryOperators() const {
	return mUnaryOperators;
}

Token IntegerType::parseNumber(std::string& str, char& current, std::size_t& index) const {
	std::string num { current };
	int base = 10;

	//Check which base the number is
	if (current == '0' && (index + 1) < str.size()) {
		char baseChar = str[index + 1];

		if (baseChar == 'b') {
			base = 2;
			num = "";
			index++;
		} else if (baseChar == 'x') {
			base = 16;
			num = "";
			index++;
		}
	}

	while (true) {
		std::size_t next = index + 1;

		if (next >= str.size()) {
			break;
		}

		current = std::tolower(str[next]);

		if (base == 2) {
			//Binary
			if (!(current == '0' || current == '1')) {
				break;
			}
		} else if (base == 10) {
			//Decimal
			if (!isdigit(current)) {
				break;
			}
		} else {
			//Hex
			if (!(isdigit(current)
				  || current == 'a' || current == 'b' || current == 'c'
				  || current == 'd' || current == 'e' || current == 'f')) {
				break;
			}
		}

		num += current;
		index = next;
	}

	return parseInt64(num, base);
}

//Float type
FloatType::FloatType() {
	mBinaryOperators = {
		 { '^', Operator('^', 6, OperatorAssociativity::RIGHT, [&](ResultValue lhs, ResultValue rhs) {
			 return pow(lhs.floatValue(), rhs.floatValue());
		 }) },
		 { '*', Operator('*', 5, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.floatValue() * rhs.floatValue();
		 }) },
		 { '/', Operator('/', 5, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.intValue() / rhs.floatValue();
		 }) },
		 { '+', Operator('+', 4, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.floatValue() + rhs.floatValue();
		 }) },
		 { '-', Operator('-', 4, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.floatValue() - rhs.floatValue();
		 }) },
		 { '=', Operator('=', 1, OperatorAssociativity::RIGHT, [&](ResultValue lhs, ResultValue rhs) {
			 return ResultValue();
		 }) }
	 };

	mUnaryOperators = {
		{ '-', Operator('-', 7, OperatorAssociativity::LEFT, [&](ResultValue op) {
			return -op.floatValue();
		}) }
	};
}

const BinaryOperators& FloatType::binaryOperators() const {
	return mBinaryOperators;
}

const UnaryOperators& FloatType::unaryOperators() const {
	return mUnaryOperators;
}

Token FloatType::parseNumber(std::string& str, char& current, std::size_t& index) const {
	std::string num { current };
	bool hasDecimalPoint = false;

	while (true) {
		std::size_t next = index + 1;

		if (next >= str.size()) {
			break;
		}

		current = std::tolower(str[next]);

		if (current == '.') {
			if (!hasDecimalPoint) {
				hasDecimalPoint = true;
			} else {
				throw std::runtime_error("The token already contains a decimal point.");
			}
		} else {
			if (!isdigit(current)) {
				break;
			}
		}

		num += current;
		index = next;
	}

	return std::stod(num);
}
