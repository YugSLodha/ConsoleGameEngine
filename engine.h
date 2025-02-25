
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
	static std::random_device dev;
	static std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(start, end);
	return dist(rng);
}

void clearScreen() {
	std::cout << "\033[2J\033[1;1H";
}

class Sprite {
public:
	std::vector<std::vector<char>> texture;
	int width, height, xpos, ypos;
	int color;

	void setTexture(const std::vector<std::vector<char>>& texture, int xpos = 0, int ypos = 0, int color = 7) {
		this->texture = texture;
		this->width = texture[0].size();
		this->height = texture.size();
		this->xpos = xpos;
		this->ypos = ypos;
		this->color = color;
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

	void drawBorder(int width = 80, int height = 25, char borderChar = '#', int borderColor = 7) {
		// Draw top and bottom borders
		for (int x = 0; x < width; ++x) {
			drawChar(x, 0, borderChar, borderColor); // Top border
			drawChar(x, height - 1, borderChar, borderColor); // Bottom border
		}

		// Draw left and right borders
		for (int y = 0; y < height; ++y) {
			drawChar(0, y, borderChar, borderColor); // Left border
			drawChar(width - 1, y, borderChar, borderColor); // Right border
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

	bool isKeyPressed(char key) {
		return GetAsyncKeyState(static_cast<int>(key)) & 0x8000;
	}
};



// FpsManager Class
class FpsManager {
	int targetFps;
	std::chrono::time_point<std::chrono::steady_clock> lastFrameTime;
	float deltaTime;  // Time elapsed since the last frame

public:
	FpsManager(int fps) : targetFps(fps), deltaTime(0.0f) {
		lastFrameTime = std::chrono::steady_clock::now();
	}

	float regulate() {
		using namespace std::chrono;

		auto currentTime = steady_clock::now();
		auto elapsedTime = duration_cast<duration<float>>(currentTime - lastFrameTime);
		deltaTime = elapsedTime.count();

		auto targetFrameDuration = duration<float>(1.0f / targetFps);
		if (elapsedTime < targetFrameDuration) {
			std::this_thread::sleep_for(targetFrameDuration - elapsedTime);
		}

		lastFrameTime = steady_clock::now();
		return deltaTime;
	}

};


#endif
