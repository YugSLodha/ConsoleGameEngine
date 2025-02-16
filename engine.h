
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
#include "bigfont.h"

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

std::vector<std::string> getTitleFont(char letter) {
	letter = (char)tolower(letter);
	switch (letter) {
	case 'a': return a;
	case 'b': return b;
	case 'c': return c;
	case 'd': return d;
	case 'e': return e;
	case 'f': return f;
	case 'g': return g;
	case 'h': return h;
	case 'i': return i;
	case 'j': return j;
	case 'k': return k;
	case 'l': return l;
	case 'm': return m;
	case 'n': return n;
	case 'o': return o;
	case 'p': return p;
	case 'q': return q;
	case 'r': return r;
	case 's': return s;
	case 't': return t;
	case 'u': return u;
	case 'v': return v;
	case 'w': return w;
	case 'x': return x;
	case 'y': return y;
	case 'z': return z;
	default: return std::vector<std::string>();
	}
}

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

	void move(int dx, int dy) {
		xpos += dx;
		ypos += dy;
	}
};

class Title {
	std::vector<std::string> title;
	int xpos, ypos;
	int color;

public:
	Title(std::vector<std::string>& title, int xpos, int ypos, int color) : title(title), xpos(xpos), ypos(ypos), color(color) {}
};

class UiElement {
	std::vector<std::string> text;
	int xpos, ypos;
	int color;
public:
	UiElement(const std::vector<std::string>& text, int xpos, int ypos, int color) : text(text), xpos(xpos), ypos(ypos), color(color) {}
};

class UI {
	std::vector<Title> titles;
	std::vector<UiElement> elements;

public:
	void addTitle(Title title) {
		titles.push_back(title);
	}

	void addElement(UiElement element) {
		elements.push_back(element);
	}

	void draw() {
		for (auto& title : titles) {
			for (int i = 0; i < title.title.size(); i++) {
				COORD coord = { static_cast<short>(title.xpos), static_cast<short>(title.ypos + i) };
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), title.color);
				std::cout << title.title[i];
			}
		}
	}
}:

class PhysicsObject {
public:
	Sprite sprite;
	float velocityX, velocityY;
	float mass;

	// Constructor
	PhysicsObject(Sprite gSprite, float mass, float velocityX = 0.0f, float velocityY = 0.0f)
		: sprite(gSprite), mass(mass), velocityX(velocityX), velocityY(velocityY) {
	}

	// Update position based on velocity
	void updatePosition(float deltaTime) {
		sprite.xpos += velocityX * deltaTime;
		sprite.ypos += velocityY * deltaTime;
	}
};

// PhysicsEngine Class
class PhysicsEngine {
public:
	float gravity = 9.8f;
	float dampingFactor = 0.98f;

	void applyGravity(PhysicsObject& object, float deltaTime) {
		if (object.mass > 0) {
			object.velocityY += gravity * deltaTime;
			object.updatePosition(deltaTime);
		}
	}

	void applyForce(PhysicsObject& object, float deltaTime, float forceX, float forceY) {
		if (object.mass > 0) {
			object.velocityX += (forceX / object.mass) * deltaTime;
			object.velocityY += (forceY / object.mass) * deltaTime;
		}
	}

	void applyDamping(PhysicsObject& object) {
		object.velocityX *= dampingFactor;
		object.velocityY *= dampingFactor;
	}

	void stopForces(PhysicsObject& physicsObject) {
		physicsObject.velocityY = 0;  // Stop falling
		physicsObject.velocityX = 0;  // Stop moving horizontally
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
		return { ' ', 7 };
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
