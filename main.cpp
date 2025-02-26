#include "engine.h"

int main() {
	const int width = 10;
	const int height = 5;
	const int FPS = 60;
	double deltatime;

	Renderer renderer(width, height);
	FPSManager fps(FPS);

	clearScreen();
	while (true) {
		renderer.clearBuffer();

		renderer.drawChar(2, 2, '*', 7);

		renderer.drawBuffer();
		deltatime = fps.regulate();
	}
	return 0;
}