#include "engine.h"

int main() {
	const int width = 30;
	const int height = 10;
	const int FPS = 120;
	float deltatime;

	std::vector<std::vector<char>> texture = { {'D', 'V', 'D'} };

	FpsManager fps(FPS);
	Renderer renderer(width, height);

	Sprite sprite;
	sprite.setTexture(texture, 1, 1, 6); =

		clearScreen();
	while (true) {
		deltatime = fps.regulate();
		renderer.clearBuffer();
		renderer.drawBorder(width, height, '#', 15);

		renderer.drawSprite(sprite);

		renderer.drawBuffer();
	}

	return 0;
}