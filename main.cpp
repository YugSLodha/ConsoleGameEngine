#include "engine.h"

int main() {
	const int FPS = 60;
	const int width = 30;
	const int height = 20;

	Renderer renderer(width, height);
	FpsManager fps(FPS);
	Input input;

	renderer.resizeConsole(width, height);
	clearScreen();
	while (true) {
		renderer.clearBuffer();
		float deltatime = fps.regulate();
		renderer.drawBorder(width, height, '#', 15);
		// CODE START

		// CODE END
		renderer.drawBuffer();
	}
	return 0;
}