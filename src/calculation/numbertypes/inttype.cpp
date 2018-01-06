#include "inttype.h"
#include "../numberhelpers.h"

//Integer type
namespace {
	//Parses a 64 bits integer in the given base
	std::int64_t parseInt64(std::string str, int base) {
		#if defined(_WIN64) || defined(__MINGW32__)
		return std::stoll(str, nullptr, base);
		#else
		return std::stol(str, nullptr, base);
		#endif
	}
}

IntegerType::IntegerType(std::ostream& os) {
	mBinaryOperators = {
		{ '^', Operator::binary('^', 6, OperatorAssociativity::RIGHT, [&](ResultValue lhs, ResultValue rhs) {
			return power(lhs.intValue(), rhs.intValue());
		}) },
		{ '*', Operator::binary('*', 5, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.intValue() * rhs.intValue();
		}) },
		{ '/', Operator::binary('/', 5, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.intValue() / rhs.intValue();
		}) },
		{ '%', Operator::binary('%', 5, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.intValue() % rhs.intValue();
		}) },
		{ '+', Operator::binary('+', 4, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.intValue() + rhs.intValue();
		}) },
		{ '-', Operator::binary('-', 4, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.intValue() - rhs.intValue();
		}) },
		{ OperatorChar('<', '<'), Operator::binary(OperatorChar('<', '<'), 3, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.intValue() << rhs.intValue();
		}) },
		{ OperatorChar('>', '>'), Operator::binary(OperatorChar('>', '>'), 3, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.intValue() >> rhs.intValue();
		}) },
		{ '|', Operator::binary('|', 2, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.intValue() | rhs.intValue();
		}) },
		{ '&', Operator::binary('&', 2, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.intValue() & rhs.intValue();
		}) },
		{ '=', Operator::binary('=', 1, OperatorAssociativity::RIGHT, [&](ResultValue lhs, ResultValue rhs) {
			return ResultValue();
		}) }
	};

	mUnaryOperators = {
		{ '-', Operator::unary('-', 7, OperatorAssociativity::LEFT, [&](ResultValue op) {
			return -op.intValue();
		}) },
		{ '~', Operator::unary('~', 7, OperatorAssociativity::LEFT, [&](ResultValue op) {
			return ~op.intValue();
		}) },
	};

	mEnvironment = EnvironmentScope(Variables({}), {
		Function("bitand", 2, [this](FunctionArguments args) {
			return ResultValue(args.at(0).intValue() ^ args.at(1).intValue());
		}, "Computes bitwise AND between x and y."),
		Function("bitor", 2, [this](FunctionArguments args) {
			return ResultValue(args.at(0).intValue() | args.at(1).intValue());
		}, "Computes bitwise OR between x and y."),
		Function("bitxor", 2, [this](FunctionArguments args) {
			return ResultValue(args.at(0).intValue() ^ args.at(1).intValue());
		}, "Computes bitwise XOR between x and y."),
		Function("mod", 2, [this](FunctionArguments args) {
			std::int64_t result = args.at(0).intValue() % args.at(1).intValue();

			if (result < 0) {
				result += args.at(1).intValue();
			}

			return ResultValue(result);
		}, "Computes x mod y."),
		Function("powerMod", 3, [this](FunctionArguments args) {
			return ResultValue(
				powerMod(args.at(0).intValue(),
						 args.at(1).intValue(),
						 args.at(2).intValue()));
		}, "Computes x^y mod z."),
		Function("gcd", 2, [this](FunctionArguments args) {
			return ResultValue(gcd(args.at(0).intValue(), args.at(1).intValue()));
		}, "Computes the greatest common divisor between x and y."),
		Function("modInv", 2, [this](FunctionArguments args) {
			if (gcd(args[0].intValue(), args[1].intValue()) == 1) {
				return ResultValue(
					modularMultInverse(
						args.at(0).intValue(),
						args.at(1).intValue()));
			} else {
				throw std::runtime_error(
					std::to_string(args[0].intValue()) + " is not invertible mod " + std::to_string(args[1].intValue()) + ".");
			}
		}, "Tries to find the multiplicative inverse of x mod y."),
		Function("sqrt", 1, [this](FunctionArguments args) {
			if (args[0].intValue() >= 0) {
				return ResultValue(sqrt(args[0].intValue()));
			} else {
				throw std::runtime_error("The value must be >= 0.");
			}
		}, "Returns the integer part of the square root of x."),
		Function("twocomp", 1, [this, &os](FunctionArguments args) {
			os << "0b" << NumberHelpers::toBaseBinary(args.at(0).intValue()) << std::endl;
			return ResultValue();
		}, "Prints the given value in two-complements form."),
		Function("twocomp", 2, [this, &os](FunctionArguments args) {
			int size = (int)args.at(1).intValue();
			os << "0b" << NumberHelpers::toBaseBinary(args.at(0).intValue(), size) << std::endl;
			return ResultValue();
		}, "Prints the value x in two-complements form of size y."),
	});
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

std::int64_t IntegerType::powerMod(std::int64_t a, std::int64_t b, std::int64_t n) {
	//From: https://en.wikipedia.org/wiki/Modular_exponentiation#Right-to-left_binary_method
	std::int64_t res = 1;
	std::int64_t base = a;

	while (b > 0) {
		if (b % 2 == 1) {
			res = (res * base) % n;
		}

		b /= 2;
		base = (base * base) % n;
	}

	return res;
}

std::int64_t IntegerType::modularMultInverse(std::int64_t x, std::int64_t n) {
	//From http://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
	std::int64_t t = 0;
	std::int64_t newT = 1;
	std::int64_t r = n;
	std::int64_t newR = x;

	while (newR != 0) {
		auto q = r / newR;

		auto tmp = t;
		t = newT;
		newT = tmp - q * newT;

		tmp = r;
		r = newR;
		newR = tmp - q * newR;

		t += n;
	}

	//To avoid a negative answer
	return (t % n + n) % n;
}

std::int64_t IntegerType::gcd(std::int64_t x, std::int64_t y) {
	//Using euclidean algorithm
	std::int64_t reminder;

	while (y != 0) {
		reminder = x % y;
		x = y;
		y = reminder;
	}

	return x;
}

std::int64_t IntegerType::sqrt(std::int64_t x) {
	//From: http://stackoverflow.com/questions/1100090/looking-for-an-efficient-integer-square-root-algorithm-for-arm-thumb2
	std::uint64_t op = (std::uint64_t)x;
	std::uint64_t res = 0;
	std::uint64_t one = 1ULL << 62;

	// "one" starts at the highest power of four <= than the argument.
	while (one > op) {
		one >>= 2;
	}

	while (one != 0) {
		if (op >= res + one) {
			op = op - (res + one);
			res = res +  2 * one;
		}
		res >>= 1;
		one >>= 2;
	}

	return res;
}

const BinaryOperators& IntegerType::binaryOperators() const {
	return mBinaryOperators;
}

const UnaryOperators& IntegerType::unaryOperators() const {
	return mUnaryOperators;
}

const EnvironmentScope& IntegerType::environment() const {
	return mEnvironment;
}

bool IntegerType::isStartOfNumber(const std::string& str, char current, std::size_t index) const {
	return isdigit(current);
}

Token IntegerType::parseNumber(std::string& str, char& current, std::size_t& index) const {
	NumericChars chars({ NumericConstantChars::getChar(current) });
	unsigned char base = 10;

	//Check which base the number is
	if (current == '0' && (index + 1) < str.size()) {
		char baseChar = str[index + 1];

		if (baseChar == 'b') {
			base = 2;
			chars.clear();
			index++;
		} else if (baseChar == 'x') {
			base = 16;
			chars.clear();
			index++;
		}
	}

	while (true) {
		std::size_t next = index + 1;

		if (next >= str.size()) {
			break;
		}

		current = (char)std::tolower(str[next]);

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

		chars.push_back(NumericConstantChars::getChar(current));
		index = next;
	}

	NumericConstant numConstant(chars);
	numConstant.base(base);
	return numConstant;
}

ResultValue IntegerType::toResultValue(const NumericConstant& numericConstant) const {
	std::string value = "";

	for (std::size_t i = 0; i < numericConstant.chars().size(); i++) {
		auto c = numericConstant.chars()[i];
		bool exit = false;

		switch (c) {
			case NumericConstantChar::Zero:
			case NumericConstantChar::One:
			case NumericConstantChar::Two:
			case NumericConstantChar::Three:
			case NumericConstantChar::Four:
			case NumericConstantChar::Five:
			case NumericConstantChar::Six:
			case NumericConstantChar::Seven:
			case NumericConstantChar::Eight:
			case NumericConstantChar::Nine:
			case NumericConstantChar::A:
			case NumericConstantChar::B:
			case NumericConstantChar::C:
			case NumericConstantChar::D:
			case NumericConstantChar::E:
			case NumericConstantChar::F:
				value += NumericConstantChars::getChar(c);
				break;
			case NumericConstantChar::MinusSign:
			case NumericConstantChar::PlusSign:
				if (i > 0) {
					exit = true;
				} else {
					value += NumericConstantChars::getChar(c);
				}
				break;
			case NumericConstantChar::DecimalPoint:
				exit = true;
				break;
			case NumericConstantChar::ImaginaryUnit:
				exit = true;
				break;
		}

		if (exit) {
			break;
		}
	}

	try {
		return parseInt64(value, numericConstant.base());
	} catch (std::exception& e) {
		throw std::out_of_range("The given number is too large.");
	}
}