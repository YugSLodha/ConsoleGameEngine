#include <iostream>
#include <windows.h>
#include <chrono>
#include <vector>
#include <thread>
#include <conio.h>

class Sprite {
public:
	std::vector<std::vector<char>> texture;
	int width, height;

	void setTexture(std::vector<std::vector<char>> VVtexture, int Iwidth, int Iheight) {
		this->texture = VVtexture;
		this->width = Iwidth;
		this->height = Iheight;
	}
};

class Renderer {
	HANDLE consoleHandle;

public:
	Renderer() {
		consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(consoleHandle, &cursorInfo);
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(consoleHandle, &cursorInfo);
	}

	void setCursorPosition(int x, int y) {
		COORD position = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
		SetConsoleCursorPosition(consoleHandle, position);
	}

	void drawChar(int x, int y, char ch) {
		setCursorPosition(x, y);
		std::cout << ch;
	}

	void drawBorder(int width, int height) {
		// Draw top and bottom borders
		for (int x = 0; x < width; x++) {
			drawChar(x, 0, '#'); // Top border
			drawChar(x, height - 1, '#'); // Bottom border
		}

		// Draw left and right borders
		for (int y = 0; y < height; y++) {
			drawChar(0, y, '#'); // Left border
			drawChar(width - 1, y, '#'); // Right border
		}
	}

	void printSprite(int x, int y, Sprite& sprite) {
		for (int dy = 0; dy < sprite.height; dy++) {
			for (int dx = 0; dx < sprite.width; dx++) {
				drawChar(x + dx, y + dy, sprite.texture[dy][dx]);
			}
		}
	}

	void clearSprite(int x, int y, Sprite& sprite) {
		for (int dy = 0; dy < sprite.height; dy++) {
			for (int dx = 0; dx < sprite.width; dx++) {
				drawChar(x + dx, y + dy, ' ');
			}
		}
	}
};

int main() {
	const int width = 156;
	const int height = 41;

	const int FPS = 30;
	const int frameDuration = 1000 / FPS;

	Renderer renderer;

	std::vector<std::vector<char>> dvdTexture = { {'D', 'V', 'D'} };

	Sprite DVD;
	DVD.setTexture(dvdTexture, 3, 1);

	int x = 1, y = 1;
	int dx = 1, dy = 1;

	system("cls");
	while (true) {
		auto frameStart = std::chrono::steady_clock::now();
		renderer.drawBorder(width, height);

		//Game Code Starts
		renderer.clearSprite(x, y, DVD);

		if (_kbhit()) {
			char key = _getch();
			switch (key) {
			case 'w': if (y != 1) y--; break;
			case 's': if (y != height - 1 - DVD.height) y++; break;
			case 'a': if (x != 1) x--; break;
			case 'd': if (x != width - 1 - DVD.width) x++; break;
			}
		}

		renderer.printSprite(x, y, DVD);
		//Game Code Ends

		auto frameEnd = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();

		if (elapsed < frameDuration) {
			std::this_thread::sleep_for(std::chrono::milliseconds(frameDuration - elapsed));
		}
	}

	return 0;
}
