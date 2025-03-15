#include "engine.h"
#include <conio.h> // For _kbhit() and _getch()

int main() {
	// Initialize camera and renderer
	Camera camera;
	Renderer renderer(10, 10, &camera);
	Input input;
	Timer timer(10, [&renderer]() {renderer.setActiveScreen("nonMain");}, false);
	timer.start();

	// Define update function
	auto update = [&]() {
		if (input.isKeyPressed(VK_ESCAPE)) {
			exit(0); // Quit game on Escape key press
		}
		};

	// Define render function
	auto render = [&]() {
		renderer.clearBuffer();
		renderer.drawBorder('#', Color::White); // Draw border
		renderer.drawChar(Position(3, 3), 'G', Color::Red); // Draw 'G' at (3,3)
		renderer.drawBuffer();
		};

	auto render2 = [&]() {
		renderer.clearBuffer();
		renderer.drawBorder('#', Color::White); // Draw border
		renderer.drawChar(Position(3, 3), 'H', Color::Red); // Draw 'G' at (3,3)
		renderer.drawBuffer();
		};

	// Create a screen and set it active
	auto mainScreen = std::make_shared<Screen>(update, render);
	auto aScreen = std::make_shared<Screen>(update, render2);
	renderer.addScreen("main", mainScreen);
	renderer.addScreen("nonMain", aScreen);
	renderer.setActiveScreen("main");

	// Game loop
	clearScreen();
	while (true) {
		input.update();
		renderer.update();
		renderer.render();
		std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
	}

	return 0;
}
