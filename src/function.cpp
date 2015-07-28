#include "function.h"

Function::Function(std::string name, std::size_t numArgs, ApplyFunction func)
	: mName(name), mNumArgs(numArgs), mFunc(func) {

}

std::string Function::name() const {
	return mName;
}

std::size_t Function::numArgs() const {
	return mNumArgs;
}

ResultValue Function::apply(FnArgs args) const {
	return mFunc(args);
}