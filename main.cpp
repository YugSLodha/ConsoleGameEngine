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
	int xpos, ypos;

	void setTexture(std::vector<std::vector<char>> VVtexture, int Iwidth, int Iheight, int xpos = 0, int ypos = 0) {
		this->texture = VVtexture;
		this->width = Iwidth;
		this->height = Iheight;
		this->xpos = xpos;
		this->ypos = ypos;
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

	void printSprite(Sprite& sprite) {
		for (int dy = 0; dy < sprite.height; dy++) {
			for (int dx = 0; dx < sprite.width; dx++) {
				drawChar(sprite.xpos + dx, sprite.ypos + dy, sprite.texture[dy][dx]);
			}
		}
	}

	void clearSprite(Sprite& sprite) {
		for (int dy = 0; dy < sprite.height; dy++) {
			for (int dx = 0; dx < sprite.width; dx++) {
				drawChar(sprite.xpos + dx, sprite.ypos + dy, ' ');
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

	Sprite DVD;
	std::vector<std::vector<char>> dvdTexture = { {'D', 'V', 'D'} };
	const int x = 1, y = 1;

	DVD.setTexture(dvdTexture, 3, 1, x, y, FPS);

	int dx = 1, dy = 1;

	system("cls");
	while (true) {
		auto frameStart = std::chrono::steady_clock::now();
		renderer.drawBorder(width, height);

		//Game Code Starts
		renderer.clearSprite(DVD);

		if (_kbhit()) {
			char key = _getch();
			switch (key) {
			case 'w': if (DVD.ypos != 1) y--; break;
			case 's': if (DVD.xpos != height - 1 - DVD.height) y++; break;
			case 'a': if (DVD.ypos != 1) x--; break;
			case 'd': if (DVD.xpos != width - 1 - DVD.width) x++; break;
			}
		}

		renderer.printSprite(DVD);
		//Game Code Ends

		auto frameEnd = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();

		if (elapsed < frameDuration) {
			std::this_thread::sleep_for(std::chrono::milliseconds(frameDuration - elapsed));
		}
	}

	return 0;
}
