#include "numbertype.h"
#include "numberhelpers.h"
#include <cmath>
#include <complex>

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
		Function("xor", 2, [this](FnArgs args) {
			return ResultValue(args.at(0).intValue() ^ args.at(1).intValue());
		}, "Computes bitwise XOR between x and y."),
		Function("mod", 2, [this](FnArgs args) {
			std::int64_t result = args.at(0).intValue() % args.at(1).intValue();

			if (result < 0) {
				result += args.at(1).intValue();
			}

			return ResultValue(result);
		}, "Computes x mod y."),
		Function("powerMod", 3, [this](FnArgs args) {
			return ResultValue(
				powerMod(args.at(0).intValue(),
						 args.at(1).intValue(),
						 args.at(2).intValue()));
		}, "Computes x^y mod z."),
		Function("gcd", 2, [this](FnArgs args) {
			return ResultValue(gcd(args.at(0).intValue(), args.at(1).intValue()));
		}, "Computes the greatest common divisor between x and y."),
		Function("modInv", 2, [this](FnArgs args) {
			if (gcd(args[0].intValue(), args[1].intValue()) == 1) {
				return ResultValue(
					modularMultInverse(
						args.at(0).intValue(),
						args.at(1).intValue()));
			} else {
				throw std::runtime_error(std::to_string(args[0].intValue()) + " is not invertible mod " + std::to_string(args[1].intValue()) + ".");
			}
		}, "Tries to find the multiplicative inverse of x mod y."),
		Function("sqrt", 1, [this](FnArgs args) {
			if (args[0].intValue() >= 0) {
				return ResultValue(sqrt(args[0].intValue()));
			} else {
				throw std::runtime_error("The value must be >= 0.");
			}
		}, "Returns the integer part of the square root of x."),
		Function("twocomp", 1, [this, &os](FnArgs args) {
			os << "0b" << NumberHelpers::toBaseBinary(args.at(0).intValue()) << std::endl;
			return ResultValue();
		}, "Prints the given value in two-complements form."),
		Function("twocomp", 2, [this, &os](FnArgs args) {
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
	std::uint64_t op = (uint64_t)x;
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

		num += current;
		index = next;
	}

	try {
		return parseInt64(num, base);
	} catch (std::exception& e) {
		throw std::out_of_range("The given number is to large.");
	}
}

//Float type
FloatType::FloatType() {
	mBinaryOperators = {
		 { '^', Operator::binary('^', 6, OperatorAssociativity::RIGHT, [&](ResultValue lhs, ResultValue rhs) {
			 return pow(lhs.floatValue(), rhs.floatValue());
		 }) },
		 { '*', Operator::binary('*', 5, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.floatValue() * rhs.floatValue();
		 }) },
		 { '/', Operator::binary('/', 5, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.floatValue() / rhs.floatValue();
		 }) },
		 { '+', Operator::binary('+', 4, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.floatValue() + rhs.floatValue();
		 }) },
		 { '-', Operator::binary('-', 4, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			 return lhs.floatValue() - rhs.floatValue();
		 }) },
		 { '=', Operator::binary('=', 1, OperatorAssociativity::RIGHT, [&](ResultValue lhs, ResultValue rhs) {
			 return ResultValue();
		 }) }
	 };

	mUnaryOperators = {
		{ '-', Operator::unary('-', 7, OperatorAssociativity::LEFT, [&](ResultValue op) {
			return -op.floatValue();
		}) }
	};

	mEnvironment = EnvironmentScope({
		{ "pi", ResultValue(3.141592653589793238463) },
		{ "e", ResultValue(2.718281828459045235360) },
	}, {
		Function("sin", 1, [this](FnArgs args) {
			return ResultValue(sin(args.at(0).floatValue()));
		}, "Computes the sine of x."),
		Function("cos", 1, [this](FnArgs args) {
			return ResultValue(cos(args.at(0).floatValue()));
		}, "Computes the cosine of x."),
		Function("tan", 1, [this](FnArgs args) {
			return ResultValue(tan(args.at(0).floatValue()));
		}, "Computes the tangent of x."),
		Function("sqrt", 1, [this](FnArgs args) {
			return ResultValue(sqrt(args.at(0).floatValue()));
		}, "Computes the square root of x."),
		Function("asin", 1, [this](FnArgs args) {
			return ResultValue(asin(args.at(0).floatValue()));
		}, "Computes the inverse sine of x."),
		Function("acos", 1, [this](FnArgs args) {
			return ResultValue(acos(args.at(0).floatValue()));
		}, "Computes the inverse cosine of x."),
		Function("atan", 1, [this](FnArgs args) {
			return ResultValue(atan(args.at(0).floatValue()));
		}, "Computes the inverse tangent of x."),
		Function("ln", 1, [this](FnArgs args) {
			return ResultValue(log(args.at(0).floatValue()));
		}, "Computes the natural logarithm of x."),
		Function("log", 1, [this](FnArgs args) {
			return ResultValue(log10(args.at(0).floatValue()));
		}, "Computes the 10-logarithm of x."),
		Function("logb", 2, [this](FnArgs args) {
			return ResultValue(log(args.at(0).floatValue()) / log(args.at(1).floatValue()));
		}, "Computes the y-logarithm of x."),
		Function("ceil", 1, [this](FnArgs x) {
			return ResultValue(ceil(x.at(0).floatValue()));
		}, "Ceils x."),
		Function("floor", 1, [this](FnArgs x) {
			return ResultValue(floor(x.at(0).floatValue()));
		}, "Floors x."),
		Function("round", 1, [this](FnArgs x) {
			return ResultValue(round(x.at(0).floatValue()));
		}, "Rounds x."),
	});
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

		current = (char)std::tolower(str[next]);

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

bool FloatType::isStartOfNumber(const std::string& str, char current, std::size_t index) const {
	return isdigit(current);
}

const EnvironmentScope& FloatType::environment() const {
	return mEnvironment;
}

//Complex type
ComplexType::ComplexType(std::ostream& os) {
	mBinaryOperators = {
		{ '^', Operator::binary('^', 6, OperatorAssociativity::RIGHT, [&](ResultValue lhs, ResultValue rhs) {
			return pow(lhs.complexValue(), rhs.complexValue());
		}) },
		{ '*', Operator::binary('*', 5, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.complexValue() * rhs.complexValue();
		}) },
		{ '/', Operator::binary('/', 5, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.complexValue() / rhs.complexValue();
		}) },
		{ '+', Operator::binary('+', 4, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.complexValue() + rhs.complexValue();
		}) },
		{ '-', Operator::binary('-', 4, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.complexValue() - rhs.complexValue();
		}) },
		{ '=', Operator::binary('=', 1, OperatorAssociativity::RIGHT, [&](ResultValue lhs, ResultValue rhs) {
			return ResultValue();
		}) }
	};

	mUnaryOperators = {
		{ '-', Operator::unary('-', 7, OperatorAssociativity::LEFT, [&](ResultValue op) {
			return -op.complexValue();
		}) }
	};

	mEnvironment = EnvironmentScope({
		{ "pi", ResultValue(3.141592653589793238463) },
		{ "e", ResultValue(2.718281828459045235360) },
	}, {
		Function("sin", 1, [this](FnArgs args) {
			return ResultValue(sin(args.at(0).complexValue()));
		}, "Computes the sine of x."),
		Function("cos", 1, [this](FnArgs args) {
			return ResultValue(cos(args.at(0).complexValue()));
		}, "Computes the cosine of x."),
		Function("tan", 1, [this](FnArgs args) {
			return ResultValue(tan(args.at(0).complexValue()));
		}, "Computes the tangent of x."),
		Function("sqrt", 1, [this](FnArgs args) {
			return ResultValue(sqrt(args.at(0).complexValue()));
		}, "Computes the square root of x."),
		Function("asin", 1, [this](FnArgs args) {
			return ResultValue(asin(args.at(0).complexValue()));
		}, "Computes the inverse sine of x."),
		Function("acos", 1, [this](FnArgs args) {
			return ResultValue(acos(args.at(0).complexValue()));
		}, "Computes the inverse cosine of x."),
		Function("atan", 1, [this](FnArgs args) {
			return ResultValue(atan(args.at(0).complexValue()));
		}, "Computes the inverse tangent of x."),
		Function("ln", 1, [this](FnArgs args) {
			return ResultValue(log(args.at(0).complexValue()));
		}, "Computes the natural logarithm of x."),
		Function("real", 1, [this](FnArgs args) {
			return ResultValue(Complex(std::real(args.at(0).complexValue()), 0));
		}, "Returns the real part of x."),
		Function("imag", 1, [this](FnArgs args) {
			return ResultValue(Complex(std::imag(args.at(0).complexValue()), 0));
		}, "Returns the imaginary part of x."),
		Function("abs", 1, [this](FnArgs args) {
			return ResultValue(Complex(std::abs(args.at(0).complexValue())));
		}, "Computes absolute value (magnitude) of x."),
		Function("arg", 1, [this](FnArgs args) {
			return ResultValue(Complex(std::arg(args.at(0).complexValue())));
		}, "Returns the argument of x."),
		Function("conj", 1, [this](FnArgs args) {
			return ResultValue(std::conj(args.at(0).complexValue()));
		}, "Returns the conjugate of x."),
		Function("polar", 2, [this](FnArgs args) {
			return ResultValue(std::polar(
				args.at(0).floatValue(),
				args.at(1).floatValue()));
		}, "Creates a complex number from polar form (x*e^(y*i))."),
	});
}

const BinaryOperators& ComplexType::binaryOperators() const {
	return mBinaryOperators;
}

const UnaryOperators& ComplexType::unaryOperators() const {
	return mUnaryOperators;
}

Token ComplexType::parseNumber(std::string& str, char& current, std::size_t& index) const {
	if (current == 'i') {
		return Complex(0, 1);
	}

	std::string num { current };
	bool hasDecimalPoint = false;
	bool hasImagUnit = false;

	while (true) {
		std::size_t next = index + 1;

		if (next >= str.size()) {
			break;
		}

		current = (char)std::tolower(str[next]);

		if (current == '.') {
			if (!hasDecimalPoint) {
				hasDecimalPoint = true;
			} else {
				throw std::runtime_error("The token already contains a decimal point.");
			}
		} else if (current == 'i') {
			hasImagUnit = true;
			index = next;
			break;
		} else {
			if (!isdigit(current)) {
				break;
			}
		}

		num += current;
		index = next;
	}

	auto value = std::stod(num);
	if (hasImagUnit) {
		return Complex(0, value);
	} else {
		return Complex(value, 0);
	}
}

bool ComplexType::isStartOfNumber(const std::string& str, char current, std::size_t index) const {
	if (isdigit(current)) {
		return true;
	} else if (current == 'i') {
		//We only want if 'i' is separate, not part of identifier such as 'imag'.
		auto next = index + 1;
		if (next < str.length()) {
			return !isalpha(str[next]);
		} else {
			return true;
		}
 	} else {
		return false;
	}
}

const EnvironmentScope& ComplexType::environment() const {
	return mEnvironment;
}
