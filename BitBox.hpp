// THE BITBOX ENGINE
// V-0.3
// ALL RIGHTS RESERVED
#pragma once
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <windows.h>
#include <functional>
#include <atomic>
#include <memory>
#include <shellapi.h>

// Utility Functions
void hideCursor() { std::cout << "\033[?25l"; }
void showCursor() { std::cout << "\033[?25h"; }

void clearScreen() { std::cout << "\033[2J\033[1;1H"; }

void runAsAdmin() {
	// Check if already running as admin
	BOOL isAdmin = FALSE;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID adminGroup;

	if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0, &adminGroup)) {
		CheckTokenMembership(NULL, adminGroup, &isAdmin);
		FreeSid(adminGroup);
	}

	// Relaunch as admin if not already running as one
	if (!isAdmin) {
		wchar_t exePath[MAX_PATH];
		GetModuleFileNameW(NULL, exePath, MAX_PATH);

		SHELLEXECUTEINFOW sei = { sizeof(sei) };
		sei.lpVerb = L"runas";  // "runas" triggers UAC
		sei.lpFile = exePath;
		sei.nShow = SW_SHOWNORMAL;

		if (!ShellExecuteExW(&sei)) {
			MessageBoxW(NULL, L"Failed to run as administrator!", L"Error", MB_OK | MB_ICONERROR);
		}

		exit(0); // Close current instance
	}
}

struct Color {
	static const int Black = 0;
	static const int Blue = 1;
	static const int Green = 2;
	static const int Cyan = 3;
	static const int Red = 4;
	static const int Magenta = 5;
	static const int Yellow = 6;
	static const int White = 7;
	static const int Grey = 8;
	static const int BrightBlue = 9;
	static const int BrightGreen = 10;
	static const int BrightCyan = 11;
	static const int BrightRed = 12;
	static const int BrightMagenta = 13;
	static const int BrightYellow = 14;
	static const int BrightWhite = 15;
};

struct Position {
	int x, y;
	Position(int x = 0, int y = 0) : x(x), y(y) {}

	Position operator+(const Position& other) const {
		return Position(x + other.x, y + other.y);
	}
};

struct Pixel {
	char letter;
	int color;
	Pixel(char letter = ' ', int color = Color::White) : letter(letter), color(color) {}
};

// Screen Class
class Screen {
public:
	std::function<void()> loop;
	Screen(std::function<void()> loopFunc)
		: loop(loopFunc) {
	}
};

class Sprite {
public:
	std::vector<std::vector<Pixel>> texture;

	void setTextureSolidCol(std::vector<std::vector<char>> textureA, int color) {
		texture.clear(); // Add this before the loop
		for (int i = 0;i < textureA.size();i++) {
			std::vector<Pixel> row;
			for (int j = 0;j < textureA[i].size();j++) {
				Pixel pix(textureA[i][j], color);
				row.push_back(pix);
			}
			texture.push_back(row);
		}
	}
};

class UiElement {
public:
	Position position;
	std::string text;
	int color;
	UiElement(Position pos, std::string text, int color) : position(pos), text(text), color(color) {}
};

class UI {
public:
	std::vector<UiElement> elements;

	void addElement(const int x, const int y, const std::string& text, int color) {
		Position pos;
		pos.x = x;
		pos.y = y;
		elements.emplace_back(pos, text, color);
	}

	void clear() {
		elements.clear();
	}

	void updateElement(int index, const std::string& newText, int newColor) {
		if (index >= 0 && index < elements.size()) {
			elements[index].text = newText;
			elements[index].color = newColor;
		}
	}
};


// Camera Class
class Camera {
private:
	Position position;
public:
	Camera(int startX = 0, int startY = 0) : position(startX, startY) {}
	void move(int dx, int dy) { position.x += dx; position.y += dy; }
	Position getPosition() const { return position; }
};

// 🎨 Renderer Class
class Renderer {
private:
	HANDLE consoleHandle;
	int width, height;
	std::vector<std::vector<Pixel>> backBuffer;
	std::vector<std::vector<Pixel>> frontBuffer;
	Camera* camera;
	std::unordered_map<std::string, std::shared_ptr<Screen>> screens;
	std::shared_ptr<Screen> activeScreen;

public:
	Renderer(int w, int h, Camera* cam) : width(w), height(h), camera(cam) {
		consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		backBuffer.resize(height, std::vector<Pixel>(width, Pixel()));
		frontBuffer.resize(height, std::vector<Pixel>(width, Pixel()));
		hideCursor();
	}

	void setCursorPosition(Position pos) {
		COORD position = { static_cast<SHORT>(pos.x), static_cast<SHORT>(pos.y) };
		SetConsoleCursorPosition(consoleHandle, position);
		std::cout.flush();
	}

	void drawBorder(char Char, int color) {
		for (int x = 0; x < width; ++x) {
			backBuffer[0][x] = Pixel(Char, color);
			backBuffer[height - 1][x] = Pixel(Char, color);
		}
		for (int y = 0; y < height; ++y) {
			backBuffer[y][0] = Pixel(Char, color);
			backBuffer[y][width - 1] = Pixel(Char, color);
		}
	}

	void drawBuffer() {
		setCursorPosition(Position(0, 0));
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				const Pixel& newPixel = backBuffer[y][x];
				Pixel& oldPixel = frontBuffer[y][x];
				if (newPixel.letter != oldPixel.letter || newPixel.color != oldPixel.color) {
					setCursorPosition(Position(x, y));
					SetConsoleTextAttribute(consoleHandle, newPixel.color);
					std::cout << newPixel.letter;
					oldPixel = newPixel;
				}
			}
		}
		SetConsoleTextAttribute(consoleHandle, Color::White);
		std::cout.flush();
	}

	void clearBuffer() {
		for (int y = 1; y < height - 1; ++y) {
			for (int x = 1; x < width - 1; ++x) {
				backBuffer[y][x] = Pixel();
			}
		}
	}

	void drawChar(Position worldPos, char ch, int color = Color::White) {
		Position screenPos = worldPos + Position(-camera->getPosition().x, -camera->getPosition().y);
		if (screenPos.x > 0 && screenPos.x < width - 1 && screenPos.y > 0 && screenPos.y < height - 1) {
			backBuffer[screenPos.y][screenPos.x] = Pixel(ch, color);
		}
	}

	void drawUI(const UI& ui) {
		for (const auto& element : ui.elements) {
			Position pos = element.position;
			for (char ch : element.text) {
				drawChar(pos, ch, element.color);
				pos.x++; // Move to the right for the next character
			}
		}
	}

	void drawSprite(Sprite sprite, Position pos) {
		for (int i = 0; i < sprite.texture.size(); ++i) {
			for (int j = 0; j < sprite.texture[i].size(); ++j) {
				drawChar(pos + Position(j, i), sprite.texture[i][j].letter, sprite.texture[i][j].color);
			}
		}
	}

	void addScreen(const std::string& name, std::shared_ptr<Screen> screen) {
		screens[name] = screen;
	}

	void setActiveScreen(const std::string& name) {
		if (screens.find(name) != screens.end()) {
			activeScreen = screens[name];
		}
	}

	void loop() {
		if (activeScreen) { activeScreen->loop(); }
	}

	void clearAll() {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				backBuffer[y][x] = Pixel();
				frontBuffer[y][x] = Pixel();
			}
		}
	}

};

// ⏳ FPS Manager Class
class FPSManager {
private:
	int targetFPS;
	std::chrono::steady_clock::time_point lastFrameTime;

public:
	FPSManager(int fps) : targetFPS(fps), lastFrameTime(std::chrono::steady_clock::now()) {}

	double regulate() {
		auto frameDuration = std::chrono::milliseconds(1000 / targetFPS);
		auto now = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTime);

		if (elapsed < frameDuration) {
			std::this_thread::sleep_for(frameDuration - elapsed);
		}

		auto newTime = std::chrono::steady_clock::now();
		double deltaTime = std::chrono::duration<double>(newTime - lastFrameTime).count();
		lastFrameTime = newTime;
		return deltaTime;
	}
};

class Timer {
public:
	Timer(int seconds, std::function<void()> callback, bool repeatTimer = false)
		: seconds(seconds), callback(callback), repeat(repeatTimer), running(false) {
	}

	~Timer() {
		stop();
		if (timerThread.joinable()) {
			timerThread.join();
		}
	}

	void start() {
		stop(); // Ensure no existing timer is running

		running = true;
		timerThread = std::thread([this]() {
			do {
				std::this_thread::sleep_for(std::chrono::seconds(seconds));
				if (running) callback();
			} while (running && repeat);
			});

		timerThread.detach(); // Detach to avoid blocking main thread
	}

	void stop() {
		running = false; // Mark as stopped
	}

private:
	int seconds;
	std::function<void()> callback;
	bool repeat;
	std::atomic<bool> running;
	std::thread timerThread;
};

class Input {
private:
	std::unordered_map<int, bool> keyState; // Stores the state of each key

public:
	// Check if a specific key is currently being pressed
	bool isKeyDown(int key) {
		return (GetAsyncKeyState(key) & 0x8000) != 0;
	}

	// Check if a key was just pressed (i.e., not held down)
	bool isKeyPressed(int key) {
		bool currentlyPressed = isKeyDown(key);
		if (currentlyPressed && !keyState[key]) {
			keyState[key] = true;
			return true;
		}
		return false;
	}

	// Check if a key was just released
	bool isKeyReleased(int key) {
		bool currentlyPressed = isKeyDown(key);
		if (!currentlyPressed && keyState[key]) {
			keyState[key] = false;
			return true;
		}
		return false;
	}

	// Update the key state (call this in the game loop)
	void update() {
		for (auto& key : keyState) {
			key.second = isKeyDown(key.first);
		}
	}
};