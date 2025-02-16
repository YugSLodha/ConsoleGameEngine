
#include "engine.h"

int main() {
	char letters[26] = { 'h', 'e', 'l', 'l', 'o' };
	std::vector<std::string> a = getTitleFont(letters[0]);
	std::vector<std::string> b = getTitleFont(letters[1]);
	std::vector<std::string> c = getTitleFont(letters[2]);
	std::vector<std::string> d = getTitleFont(letters[3]);
	std::vector<std::string> e = getTitleFont(letters[4]);

	for (int i = 0; i < a.size(); i++) {
		std::cout << a[i] << ' ' << b[i]<< ' ' << c[i]<< ' ' << d[i]<< ' ' << e[i] << std::endl;
	}
	return 0;
};