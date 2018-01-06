#include "floattype.h"

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
										Function("sin", 1, [this](FunctionArguments args) {
											return ResultValue(sin(args.at(0).floatValue()));
										}, "Computes the sine of x."),
										Function("cos", 1, [this](FunctionArguments args) {
											return ResultValue(cos(args.at(0).floatValue()));
										}, "Computes the cosine of x."),
										Function("tan", 1, [this](FunctionArguments args) {
											return ResultValue(tan(args.at(0).floatValue()));
										}, "Computes the tangent of x."),
										Function("sqrt", 1, [this](FunctionArguments args) {
											return ResultValue(sqrt(args.at(0).floatValue()));
										}, "Computes the square root of x."),
										Function("asin", 1, [this](FunctionArguments args) {
											return ResultValue(asin(args.at(0).floatValue()));
										}, "Computes the inverse sine of x."),
										Function("acos", 1, [this](FunctionArguments args) {
											return ResultValue(acos(args.at(0).floatValue()));
										}, "Computes the inverse cosine of x."),
										Function("atan", 1, [this](FunctionArguments args) {
											return ResultValue(atan(args.at(0).floatValue()));
										}, "Computes the inverse tangent of x."),
										Function("ln", 1, [this](FunctionArguments args) {
											return ResultValue(log(args.at(0).floatValue()));
										}, "Computes the natural logarithm of x."),
										Function("log", 1, [this](FunctionArguments args) {
											return ResultValue(log10(args.at(0).floatValue()));
										}, "Computes the 10-logarithm of x."),
										Function("logb", 2, [this](FunctionArguments args) {
											return ResultValue(log(args.at(0).floatValue()) / log(args.at(1).floatValue()));
										}, "Computes the y-logarithm of x."),
										Function("ceil", 1, [this](FunctionArguments x) {
											return ResultValue(ceil(x.at(0).floatValue()));
										}, "Ceils x."),
										Function("floor", 1, [this](FunctionArguments x) {
											return ResultValue(floor(x.at(0).floatValue()));
										}, "Floors x."),
										Function("round", 1, [this](FunctionArguments x) {
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

bool FloatType::isStartOfNumber(const std::string& str, char current, std::size_t index) const {
	return isdigit(current);
}

const EnvironmentScope& FloatType::environment() const {
	return mEnvironment;
}

ResultValue FloatType::toResultValue(const NumericConstant& numericConstant) const {
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
			case NumericConstantChar::DecimalPoint:
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
			case NumericConstantChar::ImaginaryUnit:
				exit = true;
				break;
			default:
				break;
		}

		if (exit) {
			break;
		}
	}

	return std::stod(value);
}