#pragma once
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <windows.h>

void clearScreen() {
	std::cout << "\033[2J\033[1;1H";
}

void hideCursor() {
	std::cout << "\033[?25l";
};

void showCursor() {
	std::cout << "\033[?25h";
}

struct Pixel {
	char letter;
	int color;
};
class Renderer {
	private:
		HANDLE consoleHandle;
		int width, height;
		std::vector<std::vector<Pixel>> backBuffer; // New frame buffer
		std::vector<std::vector<Pixel>> frontBuffer; // Previous frame buffer
	
	public:
		Renderer(int w, int h) : width(w), height(h) {
			consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	
			// Hide cursor for smooth rendering
			hideCursor();
	
			// Initialize double buffers
			backBuffer.resize(height, std::vector<Pixel>(width, { ' ', 7 }));
			frontBuffer.resize(height, std::vector<Pixel>(width, { ' ', 7 }));
		}
	
		// Set cursor position in the console
		void setCursorPosition(int x, int y) {
			COORD position = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
			SetConsoleCursorPosition(consoleHandle, position);
			std::cout.flush(); // Ensure cursor moves instantly
		}
	
		// Draw the back buffer to the console, updating only changed pixels
		void drawBuffer() {
			setCursorPosition(0, 0);
	
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					const Pixel& newPixel = backBuffer[y][x];
					Pixel& oldPixel = frontBuffer[y][x];
	
					if (newPixel.letter != oldPixel.letter || newPixel.color != oldPixel.color) {
						setCursorPosition(x, y);
						SetConsoleTextAttribute(consoleHandle, newPixel.color);
						std::cout << newPixel.letter;
						oldPixel = newPixel; // Update front buffer
					}
				}
			}
			SetConsoleTextAttribute(consoleHandle, 7); // Reset color
			std::cout.flush();
		}
	
		// Clear the back buffer
		void clearBuffer() {
			for (auto& row : backBuffer) {
				std::fill(row.begin(), row.end(), Pixel{ ' ', 7 });
			}
		}
	
		// Draw a single character with color at the given position
		void drawChar(int x, int y, char ch, int color = 7) {
			if (x >= 0 && x < width && y >= 0 && y < height) {
				backBuffer[y][x] = { ch, color };
			}
		}
	};

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
	