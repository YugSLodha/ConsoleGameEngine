#include "engine.h"

int main() {
	const int screenWidth = 139;
	const int screenHeight = 30;
	const int FPS = 60;
	float deltatime;

	Camera camera(0, 0);
	Renderer renderer(screenWidth, screenHeight, &camera);
	FPSManager fpsManager(FPS);
	static Timer timer(1, [&camera]() {camera.move(1, 0);}, true);
	Input inputHandler;

	bool running = true;
	hideCursor();  // Hide cursor at the start

	// Start the timer (runs in the background without blocking)
	timer.start();

	clearScreen();
	while (running) {
		deltatime = fpsManager.regulate();
		renderer.clearBuffer();
		inputHandler.update();

		// Handle keyboard input
		if (inputHandler.isKeyPressed(VK_ESCAPE)) running = false;


		// Draw elements
		renderer.drawChar(Position(19, 10), '@', Color::Red);
		renderer.drawChar(Position(27, 2), '%', Color::Yellow);

		renderer.drawBorder('#', Color::BrightWhite);
		renderer.drawBuffer();
	}

	// Stop timer before exiting
	timer.stop();
	delete timer;
	showCursor();  // Restore cursor visibility

	return 0;
}
