#include "engine.h"
#include <conio.h>

int main() {
	const int screenWidth = 139;
	const int screenHeight = 30;
	const int FPS = 60;

	Camera camera(0, 0);
	Renderer renderer(screenWidth, screenHeight, &camera);
	FPSManager fpsManager(FPS);
	Timer timer;

	bool running = true;
	hideCursor();  // Hide cursor at the start

	// Start the timer (runs in the background without blocking)
	timer.start(1, [&camera]() {
		camera.move(1, 0);
		});

	clearScreen();
	while (running) {
		renderer.clearBuffer();

		// Handle keyboard input
		if (_kbhit()) {
			char key = _getch();
			if (key == 'q' || key == 'Q') running = false;
		}

		// Draw elements
		renderer.drawChar(Position(19, 10), '@', Color::Red);
		renderer.drawChar(Position(27, 2), '%', Color::Yellow);

		renderer.drawBorder('#', Color::BrightWhite);
		renderer.drawBuffer();

		deltatime = fpsManager.regulate();
	}

	// Stop timer before exiting
	timer.stop();
	showCursor();  // Restore cursor visibility

	return 0;
}
