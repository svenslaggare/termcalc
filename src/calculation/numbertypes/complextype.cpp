#include "complextype.h"

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
										Function("sin", 1, [this](FunctionArguments args) {
											return ResultValue(std::sin(args.at(0).complexValue()));
										}, "Computes the sine of x."),
										Function("cos", 1, [this](FunctionArguments args) {
											return ResultValue(std::cos(args.at(0).complexValue()));
										}, "Computes the cosine of x."),
										Function("tan", 1, [this](FunctionArguments args) {
											return ResultValue(std::tan(args.at(0).complexValue()));
										}, "Computes the tangent of x."),
										Function("sqrt", 1, [this](FunctionArguments args) {
											return ResultValue(std::sqrt(args.at(0).complexValue()));
										}, "Computes the square root of x."),
										Function("asin", 1, [this](FunctionArguments args) {
											return ResultValue(std::asin(args.at(0).complexValue()));
										}, "Computes the inverse sine of x."),
										Function("acos", 1, [this](FunctionArguments args) {
											return ResultValue(std::acos(args.at(0).complexValue()));
										}, "Computes the inverse cosine of x."),
										Function("atan", 1, [this](FunctionArguments args) {
											return ResultValue(std::atan(args.at(0).complexValue()));
										}, "Computes the inverse tangent of x."),
										Function("ln", 1, [this](FunctionArguments args) {
											return ResultValue(std::log(args.at(0).complexValue()));
										}, "Computes the natural logarithm of x."),
										Function("real", 1, [this](FunctionArguments args) {
											return ResultValue(Complex(std::real(args.at(0).complexValue()), 0));
										}, "Returns the real part of x."),
										Function("imag", 1, [this](FunctionArguments args) {
											return ResultValue(Complex(std::imag(args.at(0).complexValue()), 0));
										}, "Returns the imaginary part of x."),
										Function("abs", 1, [this](FunctionArguments args) {
											return ResultValue(Complex(std::abs(args.at(0).complexValue())));
										}, "Computes absolute value (magnitude) of x."),
										Function("arg", 1, [this](FunctionArguments args) {
											return ResultValue(Complex(std::arg(args.at(0).complexValue())));
										}, "Returns the argument of x."),
										Function("conj", 1, [this](FunctionArguments args) {
											return ResultValue(std::conj(args.at(0).complexValue()));
										}, "Returns the conjugate of x."),
										Function("polar", 2, [this](FunctionArguments args) {
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

Token ComplexType::parseNumber(std::string& str, char& current, std::size_t& index) const {
	if (current == 'i') {
		return NumericConstant({ NumericConstantChar::ImaginaryUnit });
	}

	NumericChars chars({ NumericConstantChars::getChar(current) });
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
		} else if (current == 'i') {
			index = next;
			chars.push_back(NumericConstantChar::ImaginaryUnit);
			break;
		} else {
			if (!isdigit(current)) {
				break;
			}
		}

		chars.push_back(NumericConstantChars::getChar(current));
		index = next;
	}

	return NumericConstant(chars);
}

const EnvironmentScope& ComplexType::environment() const {
	return mEnvironment;
}

ResultValue ComplexType::toResultValue(const NumericConstant& numericConstant) const {
	std::string realPart = "";
	std::string imaginaryPart = "";
	std::string& currentPart = realPart;
	bool atRealPart = true;
	bool isComplex = false;

	for (std::size_t i = 0; i < numericConstant.chars().size(); i++) {
		auto c = numericConstant.chars()[i];

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
			case NumericConstantChar::DecimalPoint:
				currentPart += NumericConstantChars::getChar(c);
				break;
			case NumericConstantChar::MinusSign:
			case NumericConstantChar::PlusSign:
				if (i > 0) {
					currentPart = imaginaryPart;
					atRealPart = false;
				} else {
					currentPart += NumericConstantChars::getChar(c);
				}
				break;
			case NumericConstantChar::ImaginaryUnit:
				if (atRealPart) {
					imaginaryPart = realPart;
					realPart = "";
				}

				isComplex = true;
				break;
			default:
				break;
		}
	}

	if (isComplex) {
		if (realPart != "") {
			return Complex(std::stod(realPart), std::stod(imaginaryPart));
		} else if (imaginaryPart != "") {
			return Complex(0.0, std::stod(imaginaryPart));
		} else {
			return Complex(0.0, 1.0);
		}
	} else {
		return Complex(std::stod(realPart), 0.0);
	}
}
