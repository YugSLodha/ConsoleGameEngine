
#include "engine.h"

int main() {
	const int FPS = 60;
	const int width = 60;
	const int height = 20;

	Renderer renderer(width, height);
	FpsManager fps(FPS);
	Input input;
	PhysicsEngine physicsEngine;

	Sprite testSprite;
	const std::vector<std::vector<char>> sTexure = { {'%', '%'}, {'%', '%'} };
	testSprite.setTexture(sTexure, 1, 1, 1, 0, 10);

	clearScreen();
	while (true) {
		renderer.clearBuffer();
		float deltatime = fps.regulate();
		renderer.drawBorder(width, height, '#', 15);
		// CODE START
		renderer.drawSprite(testSprite);
		if (testSprite.ypos < height - 3) {
			physicsEngine.applyGravity(testSprite, deltatime);
		}
		else testSprite.velocityY = 10;
		// CODE END
		renderer.drawBuffer();
	}
	return 0;
}