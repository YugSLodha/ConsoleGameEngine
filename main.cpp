#include <iostream>
#include <windows.h>
#include <chrono>
#include <vector>
#include <thread>
#include <conio.h>
#include <string>

class Sprite {
public:
	std::vector<std::vector<char>> texture;
	int width, height;
	int xpos, ypos;
	std::string color;

	void setTexture(std::vector<std::vector<char>> VVtexture, int Iwidth, int Iheight, std::string color = "original", int xpos = 0, int ypos = 0) {
		this->texture = VVtexture;
		this->width = Iwidth;
		this->height = Iheight;
		this->xpos = xpos;
		this->ypos = ypos;
		this->color = color;
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

	void drawChar(int x, int y, char ch, std::string color = "original") {
		std::string colorCode;
		if (color == "original") colorCode = "\033[0m";
		else if (color == "red") colorCode = "\033[31m";
		else if (color == "green") colorCode = "\033[32m";
		else if (color == "yellow") colorCode = "\033[33m";
		else if (color == "blue") colorCode = "\033[34m";
		else if (color == "magenta") colorCode = "\033[35m";
		else if (color == "cyan") colorCode = "\033[36m";
		else if (color == "white") colorCode = "\033[37m";
		setCursorPosition(x, y);
		std::cout << colorCode << ch;
	}

	void drawBorder(int width, int height, std::string color) {
		// Draw top and bottom borders
		for (int x = 0; x < width; x++) {
			drawChar(x, 0, '#', color); // Top border
			drawChar(x, height - 1, '#', color); // Bottom border
		}

		// Draw left and right borders
		for (int y = 0; y < height; y++) {
			drawChar(0, y, '#', color); // Left border
			drawChar(width - 1, y, '#', color); // Right border
		}
	}

	void printSprite(Sprite& sprite) {
		for (int dy = 0; dy < sprite.height; dy++) {
			for (int dx = 0; dx < sprite.width; dx++) {
				drawChar(sprite.xpos + dx, sprite.ypos + dy, sprite.texture[dy][dx], sprite.color);
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

class Input {
public:
	bool isKeyPressed(char key) {
		if (_kbhit()) {
			char pressedKey = _getch();
			if (pressedKey == key) return true;
		}
		return false;
	}

	char getKey() {
		if (_kbhit()) {
			return _getch();
		}
		return '\0';
	}

	void simpleMovementLogic(Sprite& sprite, int sHeight, int sWidth, int bSize = 1) {
		switch (getKey()) {
		case 'w': if (sprite.ypos > bSize) sprite.ypos--; break;
		case 'a': if (sprite.xpos > bSize) sprite.xpos--; break;
		case 's': if (sprite.ypos + sprite.height < sHeight - bSize) sprite.ypos++; break;
		case 'd': if (sprite.xpos + sprite.width < sWidth - bSize) sprite.xpos++; break;
		}
	}
};

int main() {
	// Screen Setup
	const int width = 156;
	const int height = 41;
	const int FPS = 30;
	const int frameDuration = 1000 / FPS;

	Renderer renderer;
	Input input;

	// DVD Sprite Setup
	Sprite DVD;
	std::vector<std::vector<char>> dvdTexture = { {'D', 'V', 'D'} };
	const int x = 1, y = 1;
	DVD.setTexture(dvdTexture, dvdTexture[0].size(), dvdTexture.size(), "blue", x, y);

	system("cls");
	while (true) {
		auto frameStart = std::chrono::steady_clock::now();
		renderer.drawBorder(width, height, "red");

		//Game Code Starts
		renderer.clearSprite(DVD);

		input.simpleMovementLogic(DVD, height, width);

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
