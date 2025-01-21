#include "engine.h"
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

class Bullet : public Sprite {
public:
};

int main() {
	const int FPS = 60;
	const int frameDuration = 1000 / FPS;
	const int width = 30;
	const int height = 20;
	int bulletCount = 0;
	std::vector<Bullet> bullets;

	Renderer renderer;
	Input input;

	Sprite shooter;
	std::vector<std::vector<char>> shooterTexture = { {'-', '-', '\\'},
													  {' ', ' ', '>'},
													  {'-', '-', '/'} };
	const int x = 1, y = 1;
	shooter.setTexture("player", shooterTexture, shooterTexture[0].size(), shooterTexture.size(), "blue", x, y);

	std::vector<std::vector<char>> bulletTexture = { {'.'} };

	clearScreen();
	while (true) {
		auto frameStart = std::chrono::high_resolution_clock::now();

		// Draw the border
		renderer.drawBorder(width, height, "magenta");

		// Game Code Starts
		renderer.clearSprite(shooter);

		// Handle shooter movement
		input.simpleMovementLogic(shooter, height, width);

		// Handle shooting
		static auto lastBulletTime = std::chrono::high_resolution_clock::now();
		auto now = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastBulletTime).count();

		if (input.isKeyPressed(VK_SPACE, 0x8000) && elapsed > 200) { // 200 ms cooldown
			lastBulletTime = now;
			bulletCount++;
			Bullet bullet;
			bullet.setTexture("bullet" + std::to_string(bulletCount), bulletTexture, bulletTexture[0].size(), bulletTexture.size(), "red", shooter.xpos + 3, shooter.ypos + 1);
			bullets.push_back(bullet);
		}

		// Update and render bullets
		for (auto it = bullets.begin(); it != bullets.end();) {
			renderer.clearSprite(*it);
			it->xpos += 1;
			if (it->xpos >= width - 1) {
				it = bullets.erase(it); // Remove bullet if out of bounds
			}
			else {
				renderer.printSprite(*it);
				++it;
			}
		}

		// Render the shooter
		renderer.printSprite(shooter);

		// Game Code Ends

		auto frameEnd = std::chrono::high_resolution_clock::now();
		auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();

		if (frameTime < frameDuration) {
			std::this_thread::sleep_for(std::chrono::milliseconds(frameDuration - frameTime));
		}
	}
	return 0;
}
