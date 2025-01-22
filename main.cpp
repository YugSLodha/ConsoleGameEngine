#include "engine.h"
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

class Bullet : public Sprite {
public:
};

void handleBullets(std::vector<Bullet>& bullets, Sprite& enemy, Renderer& renderer, Collision& collision, int width) {
	for (auto it = bullets.begin(); it != bullets.end();) {
		renderer.clearSprite(*it);
		it->xpos += 1;

		if (it->xpos >= width - 1) {
			it = bullets.erase(it);
		}
		else if (collision.isColliding(*it, enemy)) {
			std::cout << "Collision detected! Game Over.\n";
			exit(0);
		}
		else {
			renderer.printSprite(*it);
			++it;
		}
	}
}


int main() {
	const int FPS = 30;
	const int frameDuration = 1000 / FPS;
	const int width = 50;
	const int height = 20;
	int bulletCount = 0;
	std::vector<Bullet> bullets;

	Renderer renderer;
	Input input;
	FpsManager fps(FPS);
	Collision collision;

	Sprite shooter;
	std::vector<std::vector<char>> shooterTexture = { {'-', '-', '\\'},
													  {' ', ' ', '>'},
													  {'-', '-', '/'} };
	const int x = 1, y = 1;
	shooter.setTexture("player", shooterTexture, shooterTexture[0].size(), shooterTexture.size(), "blue", x, y);

	std::vector<std::vector<char>> bulletTexture = { {'.'} };

	Sprite enemy1;
	std::vector<std::vector<char>> enemy1Texture = { {'<', '-', '-'} };
	enemy1.setTexture("enemy1", enemy1Texture, enemy1Texture[0].size(), enemy1Texture.size(), "yellow", 40, 3);

	clearScreen();
	while (true) {
		renderer.drawBorder(width, height, "magenta");

		// Game Code Starts
		renderer.clearSprite(shooter);
		renderer.clearSprite(enemy1);

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

		handleBullets(bullets, enemy1, renderer, collision, width);

		renderer.printSprite(shooter);
		renderer.printSprite(enemy1);

		// Game Code Ends

		fps.regulate();
	}
	return 0;
}