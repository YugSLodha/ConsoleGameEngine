
#include "bigfont.h"
#include "engine.h"

int main() {
	const int height = 20;
	const int width = 120;
	const int FPS = 60;
	float deltatime;

	FONT font;
	Renderer renderer(width, height);
	FpsManager fpsManager(FPS);

	clearScreen();
	while (true) {
		deltatime = fpsManager.regulate();
		renderer.clearBuffer();
		renderer.drawBorder(width, height, '#', 15);

		renderer.drawUI(Ui);

		renderer.drawBuffer();
	}


	return 0;
};