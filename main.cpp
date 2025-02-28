#include "engine.h"
#include <conio.h> // For _kbhit() and _getch()

int main() {
	int screenWidth = 40;
	int screenHeight = 20;

	// Create Camera and Renderer
	Camera camera(0, 0);
	Renderer renderer(screenWidth, screenHeight, &camera);
	FPSManager fpsManager(30);

	bool running = true;

	clearScreen();
	while (running) {
		if (_kbhit()) {
			char key = _getch();
			switch (key) {
			case 'w': camera.move(0, -1); break;
			case 's': camera.move(0, 1); break;
			case 'a': camera.move(-1, 0); break;
			case 'd': camera.move(1, 0); break;
			case 'q': running = false; break; // Quit
			}
		}

		// 🎨 Clear & Render Scene
		renderer.clearBuffer();

		// Example: Draw a red '@' at world position (10,10)
		renderer.drawChar(Position(10, 10), '@', Color::Red);
		renderer.drawChar(Position(8, 2), '%', Color::Yellow);

		renderer.drawBorder('#', Color::BrightWhite);

		// 🖥️ Render Frame
		renderer.drawBuffer();

		// ⏳ Regulate FPS
		fpsManager.regulate();
	}

	showCursor(); // Restore cursor visibility
	return 0;
}
