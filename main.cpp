#include <iostream>
#include <windows.h>
#include <chrono>
#include <thread>
#include <vector>
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
		for (int x = 0; x < width; x++) {
			drawChar(x, 0, '#', color);
			drawChar(x, height - 1, '#', color);
		}

		// Draw left and right borders
		for (int y = 0; y < height; y++) {
			drawChar(0, y, '#', color);
			drawChar(width - 1, y, '#', color);
		}
	}

	void printSprite(Sprite& sprite) {
		for (int y = 0; y < sprite.height; y++) {
			for (int x = 0; x < sprite.width; x++) {
				drawChar(sprite.xpos + x, sprite.ypos + y, sprite.texture[y][x], sprite.color);
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
	const int width = 30;
	const int height = 20;

	Renderer renderer;
	Input input;

	Sprite shooter;
	std::vector<std::vector<char>> shooterTexture = { {'-', '-', '\\'},
													  {' ', ' ', '>',},
													  {'-', '-', '/',} };
	const int x = 1, y = 1;
	shooter.setTexture(shooterTexture, shooterTexture[0].size(), shooterTexture.size(), "blue", x, y);

	system("cls");
	while (true) {
		renderer.drawBorder(width, height, "red");

		//Game Code Starts
		renderer.clearSprite(shooter);

		input.simpleMovementLogic(shooter, height, width);

		// TODO REWRITE INPUT CLASS WITH WINDOWS API

		renderer.printSprite(shooter);
		//Game Code Ends
	}

	return 0;
}
