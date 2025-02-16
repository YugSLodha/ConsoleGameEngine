#include "engine.h"

int main() {
	const int width = 10;
	const int height = 5;
	const int FPS = 60;
	double deltatime;

	Renderer renderer(width, height);
	FPSManager fps(FPS);

	while (true) {
		deltatime = fps.regulate();
		clearScreen();
		renderer.clearBuffer();

		renderer.writeToBuffer('#', 2, 2, 13);

		renderer.drawBuffer();
	}
	return 0;
}