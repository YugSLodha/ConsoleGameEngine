
#include "engine.h"

int main() {
	std::vector<std::string> H = getTitleFont('b');

	for (int i = 0; i < H.size(); i++) {
        std::cout << H[i]<< std::endl;
	}
	
	return 0;
};