#include "engine.h"
#include <conio.h>

int main() {
	const int screenWidth = 139;
	const int screenHeight = 30;
	const int FPS = 60;

	Camera camera(0, 0);
	Renderer renderer(screenWidth, screenHeight, &camera);
	FPSManager fpsManager(FPS);

	bool running = true;

	clearScreen();
	while (running) {
		if (_kbhit()) {
			char key = _getch();
			switch (key) {
			case 'a': camera.move(-1, 0); break;
			case 'd': camera.move(1, 0); break;
			case 'q': running = false; break; // Quit
			}
		}
		renderer.clearBuffer();

		renderer.drawChar(Position(10, 10), '@', Color::Red);
		renderer.drawChar(Position(8, 2), '%', Color::Yellow);

		renderer.drawBorder('#', Color::BrightWhite);
		renderer.drawBuffer();
		fpsManager.regulate();
	}

	showCursor(); // Restore cursor visibility
	return 0;
}
