#include <iostream>
#include <windows.h>
#include <chrono>
#include <thread>
#include <vector>
#include <conio.h>
#include <string>

int clearScreen() {
	system("cls");
	return 0;
}

class Sprite {
public:
	std::vector<std::vector<char>> texture;
	int width, height;
	int xpos, ypos;
	std::string color;

	void setTexture(std::string name, std::vector<std::vector<char>> VVtexture, int Iwidth, int Iheight, std::string color = "original", int xpos = 0, int ypos = 0) {
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
		else colorCode = "\033[0m";
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
	char getKey() {
		if (_kbhit()) {
			return _getch();
		}
		return '\0';
	}

	bool isKeyPressed(char key, int code) {
		if (GetAsyncKeyState(key) & code) {
			return true;
		}
		return false;
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

class Collision {
public:
	bool isColliding(const Sprite& sprite1, const Sprite& sprite2) {
		// Check for overlap between the bounding boxes of the two sprites
		bool xOverlap = sprite1.xpos < sprite2.xpos + sprite2.width &&
			sprite1.xpos + sprite1.width > sprite2.xpos;
		bool yOverlap = sprite1.ypos < sprite2.ypos + sprite2.height &&
			sprite1.ypos + sprite1.height > sprite2.ypos;

		return xOverlap && yOverlap;
	}
};

class FpsManager {
private:
	int targetFps; // Target frames per second
	std::chrono::time_point<std::chrono::steady_clock> lastFrameTime;

public:
	// Constructor
	FpsManager(int fps) : targetFps(fps) {
		lastFrameTime = std::chrono::steady_clock::now();
	}

	// Function to regulate FPS
	void regulate() {
		using namespace std::chrono;

		// Calculate the target frame duration
		auto targetFrameDuration = duration<double>(1.0 / targetFps);

		// Measure the time elapsed since the last frame
		auto currentTime = steady_clock::now();
		auto elapsedTime = duration_cast<duration<double>>(currentTime - lastFrameTime);

		if (elapsedTime < targetFrameDuration) {
			auto sleepDuration = targetFrameDuration - elapsedTime;
			std::this_thread::sleep_for(sleepDuration);
		}

		lastFrameTime = steady_clock::now();
	}
};