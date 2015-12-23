#include "numbertype.h"
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

	mEnvironment = EnvironmentScope(Variables({}), {
		{ "xor", Function("xor", 2, [this](FnArgs args) {
			return ResultValue(args.at(0).intValue() ^ args.at(1).intValue());
		}, "Bitwise XOR between x and y.") },
		{ "mod", Function("mod", 2, [this](FnArgs args) {
			std::int64_t result = args.at(0).intValue() % args.at(1).intValue();

			if (result < 0) {
				result += args.at(1).intValue();
			}

			return ResultValue(result);
		}, "Computes x mod y.") },
		{ "gcd", Function("gcd", 2, [this](FnArgs args) {
			auto x = args.at(0).intValue();
			auto y = args.at(1).intValue();

			std::int64_t reminder;

			while (y != 0) {
				reminder = x % y;
				x = y;
				y = reminder;
			}

			return ResultValue(x);
		}, "Computes the greatest common divisor between x and y.") },
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
			 return lhs.floatValue() / rhs.floatValue();
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

	mEnvironment = EnvironmentScope({
		{ "pi", ResultValue(3.141592653589793238463) },
		{ "e", ResultValue(2.718281828459045235360) },
	}, {
		{ "sin", Function("sin", 1, [this](FnArgs args) {
			return ResultValue(sin(args.at(0).floatValue()));
		}, "Computes the sine of x.") },
		{ "cos", Function("cos", 1, [this](FnArgs args) {
			return ResultValue(cos(args.at(0).floatValue()));
		}, "Computes the cosine of x.") },
		{ "tan", Function("tan", 1, [this](FnArgs args) {
			return ResultValue(tan(args.at(0).floatValue()));
		}, "Computes the tangent of x.") },
		{ "sqrt", Function("sqrt", 1, [this](FnArgs args) {
			return ResultValue(sqrt(args.at(0).floatValue()));
		}, "Computes the square root of x.") },
		{ "asin", Function("asin", 1, [this](FnArgs args) {
			return ResultValue(asin(args.at(0).floatValue()));
		}, "Computes the inverse sine of x.") },
		{ "acos", Function("acos", 1, [this](FnArgs args) {
			return ResultValue(acos(args.at(0).floatValue()));
		}, "Computes the inverse cosine of x.") },
		{ "atan", Function("atan", 1, [this](FnArgs args) {
			return ResultValue(atan(args.at(0).floatValue()));
		}, "Computes the inverse tangent of x.") },
		{ "ln", Function("ln", 1, [this](FnArgs args) {
			return ResultValue(log(args.at(0).floatValue()));
		}, "Computes the natural logarithm of x.") },
		{ "log", Function("log", 1, [this](FnArgs args) {
			return ResultValue(log10(args.at(0).floatValue()));
		}, "Computes the 10-logarithm of x.") },
		{ "logb", Function("logb", 2, [this](FnArgs args) {
			return ResultValue(log(args.at(0).floatValue()) / log(args.at(1).floatValue()));
		}, "Computes the y-logarithm of x.") },
		{ "ceil", Function("ceil", 1, [this](FnArgs x) {
			return ResultValue(ceil(x.at(0).floatValue()));
		}, "Ceils x.") },
		{ "floor", Function("floor", 1, [this](FnArgs x) {
			return ResultValue(floor(x.at(0).floatValue()));
		}, "Floors x.") },
		{ "round", Function("round", 1, [this](FnArgs x) {
			return ResultValue(round(x.at(0).floatValue()));
		}, "Rounds x.") },
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
ComplexType::ComplexType() {
	mBinaryOperators = {
		{ '^', Operator('^', 6, OperatorAssociativity::RIGHT, [&](ResultValue lhs, ResultValue rhs) {
			return pow(lhs.complexValue(), rhs.complexValue());
		}) },
		{ '*', Operator('*', 5, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.complexValue() * rhs.complexValue();
		}) },
		{ '/', Operator('/', 5, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.complexValue() / rhs.complexValue();
		}) },
		{ '+', Operator('+', 4, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.complexValue() + rhs.complexValue();
		}) },
		{ '-', Operator('-', 4, OperatorAssociativity::LEFT, [&](ResultValue lhs, ResultValue rhs) {
			return lhs.complexValue() - rhs.complexValue();
		}) },
		{ '=', Operator('=', 1, OperatorAssociativity::RIGHT, [&](ResultValue lhs, ResultValue rhs) {
			return ResultValue();
		}) }
	};

	mUnaryOperators = {
		{ '-', Operator('-', 7, OperatorAssociativity::LEFT, [&](ResultValue op) {
			return -op.complexValue();
		}) }
	};

	mEnvironment = EnvironmentScope({
		{ "pi", ResultValue(3.141592653589793238463) },
		{ "e", ResultValue(2.718281828459045235360) },
	}, {
		{ "sin", Function("sin", 1, [this](FnArgs args) {
			return ResultValue(sin(args.at(0).complexValue()));
		}, "Computes the sine of x.") },
		{ "cos", Function("cos", 1, [this](FnArgs args) {
			return ResultValue(cos(args.at(0).complexValue()));
		}, "Computes the cosine of x.") },
		{ "tan", Function("tan", 1, [this](FnArgs args) {
			return ResultValue(tan(args.at(0).complexValue()));
		}, "Computes the tangent of x.") },
		{ "sqrt", Function("sqrt", 1, [this](FnArgs args) {
			return ResultValue(sqrt(args.at(0).complexValue()));
		}, "Computes the square root of x.") },
		{ "asin", Function("asin", 1, [this](FnArgs args) {
			return ResultValue(asin(args.at(0).complexValue()));
		}, "Computes the inverse sine of x.") },
		{ "acos", Function("acos", 1, [this](FnArgs args) {
			return ResultValue(acos(args.at(0).complexValue()));
		}, "Computes the inverse cosine of x.") },
		{ "atan", Function("atan", 1, [this](FnArgs args) {
			return ResultValue(atan(args.at(0).complexValue()));
		}, "Computes the inverse tangent of x.") },
		{ "ln", Function("ln", 1, [this](FnArgs args) {
			return ResultValue(log(args.at(0).complexValue()));
		}, "Computes the natural logarithm of x.") },
		{ "real", Function("real", 1, [this](FnArgs args) {
			return ResultValue(Complex(std::real(args.at(0).complexValue()), 0));
		}, "Returns the real part of x.") },
		{ "imag", Function("imag", 1, [this](FnArgs args) {
			return ResultValue(Complex(std::imag(args.at(0).complexValue()), 0));
		}, "Returns the imaginary part of x.") },
		{ "abs", Function("abs", 1, [this](FnArgs args) {
			return ResultValue(Complex(std::abs(args.at(0).complexValue())));
		}, "Computes absolute value (magnitude) of x.") },
		{ "arg", Function("arg", 1, [this](FnArgs args) {
			return ResultValue(Complex(std::arg(args.at(0).complexValue())));
		}, "Computes argument of x.") },
		{ "conj", Function("conj", 1, [this](FnArgs args) {
			return ResultValue(std::conj(args.at(0).complexValue()));
		}, "Computes the conjugate x.") },
		{ "polar", Function("polar", 2, [this](FnArgs args) {
			return ResultValue(std::polar(
				args.at(0).floatValue(),
				args.at(1).floatValue()));
		}, "Create a complex from polar form (x*e^(y*i)).") },
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
