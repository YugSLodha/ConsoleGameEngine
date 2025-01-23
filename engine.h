#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <conio.h>

// COLOR CHART
// 0	Black
// 1	Blue
// 2	Green
// 3	Aqua (Cyan)
// 4	Red
// 5	Purple
// 6	Yellow
// 7	White
// 8	Gray
// 9	Light Blue
// 10	Light Green
// 11	Light Aqua
// 12	Light Red
// 13	Light Purple
// 14	Light Yellow
// 15	Bright White


// Utility Functions
int randomNumber(int start, int end) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(start, end);
	return dist(rng);
}

void clearScreen() {
	system("cls");
}

// Sprite Class
class Sprite {
public:
	std::vector<std::vector<char>> texture;
	int width, height, xpos, ypos;
	int color; // Single color for the entire sprite

	void setTexture(const std::vector<std::vector<char>>& texture, int xpos = 0, int ypos = 0, int color = 7) {
		this->texture = texture;
		this->width = texture[0].size();
		this->height = texture.size();
		this->xpos = xpos;
		this->ypos = ypos;
		this->color = color;
	}
};

// Background Class
class Background {
public:
	std::vector<std::vector<char>> texture;
	std::vector<std::vector<int>> colorTexture; // Color for each cell
	int width, height;

	void setTexture(int width, int height, int color = 7) {
		this->width = width;
		this->height = height;
		texture.resize(height, std::vector<char>(width, ' '));
		colorTexture.resize(height, std::vector<int>(width, color));
	}
};

// Renderer Class
class Renderer {
	HANDLE consoleHandle;
	std::vector<std::vector<std::pair<char, int>>> buffer; // Pair of character and color

public:
	Renderer(int width, int height) {
		consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		// Hide cursor
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(consoleHandle, &cursorInfo);
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(consoleHandle, &cursorInfo);

		// Initialize buffer
		buffer.resize(height, std::vector<std::pair<char, int>>(width, { ' ', 7 }));
	}

	void setCursorPosition(int x, int y) {
		COORD position = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
		SetConsoleCursorPosition(consoleHandle, position);
	}

	void drawBuffer() {
		setCursorPosition(0, 0);
		for (const auto& row : buffer) {
			for (const auto& cell : row) {
				SetConsoleTextAttribute(consoleHandle, cell.second); // Set color
				std::cout << cell.first;
			}
			std::cout << '\n';
		}
		SetConsoleTextAttribute(consoleHandle, 7); // Reset to default color
	}

	void clearBuffer() {
		for (auto& row : buffer) {
			std::fill(row.begin(), row.end(), std::make_pair(' ', 7));
		}
	}

	void drawChar(int x, int y, char ch, int color = 7) {
		if (x >= 0 && x < buffer[0].size() && y >= 0 && y < buffer.size()) {
			buffer[y][x] = { ch, color };
		}
	}

	void drawSprite(const Sprite& sprite) {
		for (int y = 0; y < sprite.height; ++y) {
			for (int x = 0; x < sprite.width; ++x) {
				drawChar(sprite.xpos + x, sprite.ypos + y, sprite.texture[y][x], sprite.color);
			}
		}
	}

	void drawBackground(const Background& background) {
		for (int y = 0; y < background.height; ++y) {
			for (int x = 0; x < background.width; ++x) {
				drawChar(x, y, background.texture[y][x], background.colorTexture[y][x]);
			}
		}
	}
};

// Input Class
class Input {
public:
	char getKey() {
		if (_kbhit()) {
			return _getch();
		}
		return '\0';
	}

	bool isKeyPressed(char key, int code) {
		return GetAsyncKeyState(key) & code;
	}

	void simpleMovementLogic(Sprite& sprite, int screenHeight, int screenWidth, int border = 1) {
		switch (getKey()) {
		case 'w': if (sprite.ypos > border) sprite.ypos--; break;
		case 'a': if (sprite.xpos > border) sprite.xpos--; break;
		case 's': if (sprite.ypos + sprite.height < screenHeight - border) sprite.ypos++; break;
		case 'd': if (sprite.xpos + sprite.width < screenWidth - border) sprite.xpos++; break;
		}
	}
};

// FpsManager Class
class FpsManager {
	int targetFps;
	std::chrono::time_point<std::chrono::steady_clock> lastFrameTime;

public:
	FpsManager(int fps) : targetFps(fps) {
		lastFrameTime = std::chrono::steady_clock::now();
	}

	void regulate() {
		using namespace std::chrono;
		auto targetFrameDuration = duration<double>(1.0 / targetFps);
		auto currentTime = steady_clock::now();
		auto elapsedTime = duration_cast<duration<double>>(currentTime - lastFrameTime);

		if (elapsedTime < targetFrameDuration) {
			std::this_thread::sleep_for(targetFrameDuration - elapsedTime);
		}

		lastFrameTime = steady_clock::now();
	}
};

#endif