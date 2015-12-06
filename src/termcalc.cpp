#include "commandengine.h"
#include "linenoise.h"
#include <iostream>

#ifdef __unix__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

int main() {
	CommandEngine engine;
	std::string homeDir = "";

	#ifdef __unix__
	struct passwd *pw = getpwuid(getuid());
	homeDir = pw->pw_dir;
	#endif

	engine.loadFile(homeDir + "/default.termcalc", false);

	#if __unix__
		char* line;
		while((line = linenoise("> ")) != nullptr) {
			linenoiseHistoryAdd(line);
			if (engine.execute(line)) {
				break;
			}
			free(line);
		}
	#else
		std::string line;
		while (std::cin) {
			std::cout << "> ";
			std::getline(std::cin, line);
			if (engine.execute(line)) {
				break;
			}
		}
	#endif
}
