#include "engine.h"
#include <vector>

// Function to update the background texture with stars
void setBGTexture(Background& background, int width, int height, double density = 0.1) {
	// Shift columns leftward
	for (int x = 0; x < width - 1; ++x) {
		for (int y = 0; y < height; ++y) {
			background.texture[y][x] = background.texture[y][x + 1];
			background.colorTexture[y][x] = background.colorTexture[y][x + 1];
		}
	}

	// Generate a new rightmost column with controllable density
	for (int y = 0; y < height; ++y) {
		if (randomNumber(0, 100) < (density * 100)) {
			int color = randomNumber(9, 15);
			background.texture[y][width - 1] = '*';
			background.colorTexture[y][width - 1] = color;
		}
		else {
			background.texture[y][width - 1] = ' ';
			background.colorTexture[y][width - 1] = 7; // Default white background
		}
	}
}

int main() {
	const int FPS = 60;
	const int width = 131;
	const int height = 30;

	// TODO OPTIMIZE
	// TODO ADD DELTA TIME

	Renderer renderer(width, height);
	Input input;
	FpsManager fps(FPS);

	// Shooter sprite
	Sprite shooter;
	shooter.setTexture({ {'-', '-', '\\'},
						 {' ', ' ', '>'},
						 {'-', '-', '/'} }, 5, height / 2, 9); // Light blue shooter

	// Background
	Background background;
	background.setTexture(width, height, 7); // Default white background

	// Bullets
	std::vector<Sprite> bullets;
	std::vector<std::vector<char>> bulletTexture = { {'.'} };

	clearScreen();

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
			bullet.setTexture(bulletTexture, shooter.xpos + shooter.width, shooter.ypos + 1, 12); // Red bullets
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
