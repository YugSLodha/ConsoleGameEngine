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

int randomNumber(int start, int end) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(start, end);
	return dist(rng);
}

class Sprite {
public:
	std::vector<std::vector<char>> texture;
	int width, height, xpos, ypos;
	std::string color;

	void setTexture(const std::vector<std::vector<char>>& texture, int xpos = 0, int ypos = 0, const std::string& color = "original") {
		this->texture = texture;
		this->width = texture[0].size();
		this->height = texture.size();
		this->xpos = xpos;
		this->ypos = ypos;
		this->color = color;
	}
};

class Background {
public:
	std::vector<std::vector<char>> texture;
	int width, height;

	void setTexture(int width, int height) {
		this->width = width;
		this->height = height;
		texture.resize(height, std::vector<char>(width, ' '));
	}
};

class Renderer {
	HANDLE consoleHandle;
	std::vector<std::vector<char>> buffer;

public:
	Renderer(int width, int height) {
		consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(consoleHandle, &cursorInfo);
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(consoleHandle, &cursorInfo);

		buffer.resize(height, std::vector<char>(width, ' '));
	}

	void setCursorPosition(int x, int y) {
		COORD position = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
		SetConsoleCursorPosition(consoleHandle, position);
	}

	void drawBuffer() {
		setCursorPosition(0, 0);
		for (const auto& row : buffer) {
			for (const auto& ch : row) {
				std::cout << ch;
			}
			std::cout << '\n';
		}
	}

	void clearBuffer() {
		for (auto& row : buffer) {
			std::fill(row.begin(), row.end(), ' ');
		}
	}

	void drawChar(int x, int y, char ch) {
		if (x >= 0 && x < buffer[0].size() && y >= 0 && y < buffer.size()) {
			buffer[y][x] = ch;
		}
	}

	void drawSprite(const Sprite& sprite) {
		for (int y = 0; y < sprite.height; ++y) {
			for (int x = 0; x < sprite.width; ++x) {
				drawChar(sprite.xpos + x, sprite.ypos + y, sprite.texture[y][x]);
			}
		}
	}

	void drawBackground(const Background& background) {
		for (int y = 0; y < background.height; ++y) {
			for (int x = 0; x < background.width; ++x) {
				drawChar(x, y, background.texture[y][x]);
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
