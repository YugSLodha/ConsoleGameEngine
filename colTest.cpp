#include "engine.h"
#include <vector>
#include <chrono>
#include <thread>

int main() {
	Renderer renderer;
	Input input;
	Collision collision;

	// Create two sprites
	Sprite player;
	player.setTexture("Player", { {'@'} }, 1, 1, "green", 5, 5);

	Sprite enemy;
	enemy.setTexture("Enemy", { {'#'} }, 1, 1, "red", 10, 10);

	// Screen dimensions
	int screenWidth = 30;
	int screenHeight = 15;

	while (true) {
		// Clear screen
		clearScreen();

		// Draw border
		renderer.drawBorder(screenWidth, screenHeight, "cyan");

		// Draw sprites
		renderer.printSprite(player);
		renderer.printSprite(enemy);

		// Check collision
		if (collision.isColliding(player, enemy)) {
			renderer.setCursorPosition(0, screenHeight);
			std::cout << "Collision Detected!";
		}

		// Handle input for player movement
		input.simpleMovementLogic(player, screenHeight, screenWidth);

		// Small delay for smooth rendering
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}