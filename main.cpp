
#include "engine.h"

int main() {
	const int FPS = 60;
	const int width = 60;
	const int height = 20;

	Renderer renderer(width, height);
	FpsManager fps(FPS);
	Input input;
	PhysicsEngine physicsEngine;

	// Create sprite and physics object
	const std::vector<std::vector<char>> sTexture = { {'%', '%'}, {'%', '%'} };
	Sprite testSprite;
	testSprite.setTexture(sTexture, 1, 1, 1);
	PhysicsObject physicsObject(testSprite, 2.0f, 0.0f, 0.0f); // Mass = 2

	clearScreen();
	while (true) {
		renderer.clearBuffer();
		float deltaTime = fps.regulate();

		renderer.drawBorder(width, height, '#', 15);

		// Apply physics
		if (physicsObject.sprite.ypos + physicsObject.sprite.height >= height - 1) {
			physicsEngine.stopForces(physicsObject);
		}
		else {
			physicsEngine.applyGravity(physicsObject, deltaTime);
		}
		physicsEngine.applyDamping(physicsObject);

		// Render sprite
		renderer.drawSprite(physicsObject.sprite);
		renderer.drawBuffer();
	}
	return 0;
}
