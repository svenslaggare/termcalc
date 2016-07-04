#include "commandengine.h"
#include "linenoise.h"
#include <iostream>

#ifdef __unix__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

#if defined(_WIN64) || defined(__MINGW32__)
#include <windows.h>
#include <KnownFolders.h>
#include <ShlObj.h>
#include <locale>
#include <codecvt>
#endif

int main() {
	CommandEngine engine;
	std::string homeDir = "";

	#ifdef __unix__
	struct passwd *pw = getpwuid(getuid());
	homeDir = pw->pw_dir;
	#endif

	#if defined(_WIN64) || defined(__MINGW32__)
	PWSTR path = nullptr;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Profile, 0, nullptr, &path))) {
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;
		homeDir = converterX.to_bytes(std::wstring(path));
	}
	CoTaskMemFree(path);
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
