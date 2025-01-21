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
	const int FPS = 1;
	const int frameDuration = 1000 / FPS;
	const int width = 30;
	const int height = 20;
	int bulletCount = 0;
	std::vector<Bullet> bullets;

	Renderer renderer;
	Input input;
	FpsManager fps(FPS);

	Sprite shooter;
	std::vector<std::vector<char>> shooterTexture = { {'-', '-', '\\'},
													  {' ', ' ', '>'},
													  {'-', '-', '/'} };
	const int x = 1, y = 1;
	shooter.setTexture("player", shooterTexture, shooterTexture[0].size(), shooterTexture.size(), "blue", x, y);

	std::vector<std::vector<char>> bulletTexture = { {'.'} };

	clearScreen();
	while (true) {
		renderer.drawBorder(width, height, "magenta");

		// Game Code Starts
		renderer.clearSprite(shooter);

		input.simpleMovementLogic(shooter, height, width);

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

		renderer.printSprite(shooter);

		// Game Code Ends

		fps.regulate();
	}
	return 0;
}