#include <iostream>
#include "MD5.h"

int main(int argc, char* argv[])
{
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			std::cout << md5(argv[i]) << std::endl;
		}
	}
	else {
		std::string line;
		while (std::cin >> line) {
			std::cout << md5(line) << std::endl;
		}
	}
	return 0;
}