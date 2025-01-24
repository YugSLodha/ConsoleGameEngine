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
	std::vector<std::vector<std::pair<char, int>>> texture; // Each cell holds a character and its color
	int width, height;

	// Constructor to initialize the background with dimensions and optional default character and color
	void setTexture(int width, int height, char defaultChar = ' ', int defaultColor = 7) {
		this->width = width;
		this->height = height;
		texture.resize(height, std::vector<std::pair<char, int>>(width, { defaultChar, defaultColor }));
	}

	// Set a specific character and color at a given position
	void setPixel(int x, int y, char ch, int color) {
		if (x >= 0 && x < width && y >= 0 && y < height) {
			texture[y][x] = { ch, color };
		}
	}

	// Get the character and color at a specific position
	std::pair<char, int> getPixel(int x, int y) const {
		if (x >= 0 && x < width && y >= 0 && y < height) {
			return texture[y][x];
		}
		return { ' ', 7 };  // Default character and color if out of bounds
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

	void resizeConsole(int width, int height) {
		// Resize the console screen buffer
		COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
		SetConsoleScreenBufferSize(consoleHandle, bufferSize);

		// Adjust the console window size
		SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
		SetConsoleWindowInfo(consoleHandle, TRUE, &windowSize);
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
				// Retrieve the character and color from the background texture
				auto pixel = background.getPixel(x, y);
				char ch = pixel.first;
				int color = pixel.second;

				// Only update if the current pixel differs from the buffer
				if (buffer[y][x].first != ch || buffer[y][x].second != color) {
					drawChar(x, y, ch, color);  // Update the character and color at position (x, y)
				}
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
	float deltaTime;  // Time elapsed since the last frame

public:
	FpsManager(int fps) : targetFps(fps), deltaTime(0.0f) {
		lastFrameTime = std::chrono::steady_clock::now();
	}

	float regulate() {
		using namespace std::chrono;

		// Calculate time elapsed since the last frame
		auto currentTime = steady_clock::now();
		auto elapsedTime = duration_cast<duration<float>>(currentTime - lastFrameTime);
		deltaTime = elapsedTime.count();  // Store delta time in seconds

		// Calculate the target frame duration
		auto targetFrameDuration = duration<float>(1.0f / targetFps);

		// Sleep if the frame duration is less than the target frame duration
		if (elapsedTime < targetFrameDuration) {
			std::this_thread::sleep_for(targetFrameDuration - elapsedTime);
		}

		// Update the last frame time
		lastFrameTime = steady_clock::now();
		return deltaTime;
	}
};

#endif