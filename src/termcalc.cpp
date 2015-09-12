#include "commandengine.h"
#include <iostream>

#ifdef __unix__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

int main() {
	std::string line;
	CommandEngine engine;
	std::string homeDir = "";

	#ifdef __unix__
	struct passwd *pw = getpwuid(getuid());
	homeDir = pw->pw_dir;
	#endif

	engine.loadFile(homeDir + "/default.termcalc", false);

	while (std::cin) {
		std::cout << "> ";
		std::getline(std::cin, line);
		if (engine.execute(line)) {
			break;
		}
	}
}