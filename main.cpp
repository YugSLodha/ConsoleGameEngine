#include "engine.h"

int main() {
	const int FPS = 60;
	const int width = 20;
	const int height = 8;

	Renderer renderer(width, height);
	FpsManager fps(FPS);
	Input input :

	Sprite testSprite;
	testSprite.setTexture({ {'H', 'I'} }, 1, 1, 4, 1);

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