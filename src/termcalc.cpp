#include "commandengine.h"
#include <iostream>

int main() {
	std::string line;
	CommandEngine engine;

	while (std::cin) {
		std::cout << "> ";
		std::getline(std::cin, line);
		if (engine.execute(line)) {
			break;
		}
	}
}