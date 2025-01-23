#include "engine.h"
#include <vector>

void setBGTexture(Background& background, int width, int height) {
	// Shift columns leftward
	for (int x = 0; x < width - 1; ++x) {
		for (int y = 0; y < height; ++y) {
			background.texture[y][x] = background.texture[y][x + 1];
		}
	}

	// Generate a new rightmost column
	for (int y = 0; y < height; ++y) {
		background.texture[y][width - 1] = (randomNumber(0, 10) < 2) ? '*' : ' ';
	}
}

int main() {
	const int FPS = 30;
	const int width = 50;
	const int height = 20;

	Renderer renderer(width, height);
	Input input;
	FpsManager fps(FPS);

	Sprite shooter;
	shooter.setTexture({ {'-', '-', '\\'},
						 {' ', ' ', '>'},
						 {'-', '-', '/'} }, 5, height / 2, "blue");

	Background background;
	background.setTexture(width, height);

	std::vector<Sprite> bullets;
	std::vector<std::vector<char>> bulletTexture = { {'.'} };

	while (true) {
		renderer.clearBuffer();

		// Update background
		setBGTexture(background, width, height);
		renderer.drawBackground(background);

		// Handle shooter movement
		input.simpleMovementLogic(shooter, height, width);

		// Handle bullet creation
		static auto lastBulletTime = std::chrono::high_resolution_clock::now();
		auto now = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastBulletTime).count();

		if (input.isKeyPressed(VK_SPACE, 0x8000) && elapsed > 200) { // 200 ms cooldown
			lastBulletTime = now;
			Sprite bullet;
			bullet.setTexture(bulletTexture, shooter.xpos + shooter.width, shooter.ypos + 1);
			bullets.push_back(bullet);
		}

		// Update bullets
		for (auto it = bullets.begin(); it != bullets.end();) {
			it->xpos++;
			if (it->xpos >= width) {
				it = bullets.erase(it);
			}
			else {
				renderer.drawSprite(*it);
				++it;
			}
		}

		// Draw shooter
		renderer.drawSprite(shooter);

		// Render to screen
		renderer.drawBuffer();

		// Regulate FPS
		fps.regulate();
	}

	return 0;
}
