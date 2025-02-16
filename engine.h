#pragma once
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>


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
	int width, height;
	std::vector<std::vector<Pixel>> frontBuffer;
    std::vector<std::vector<Pixel>> backBuffer;

	// ANSI escape codes for basic colors
	const std::vector<std::string> ANSI_COLORS = {
		"\033[0m",    // Reset (default) 00
		"\033[30m",   // BLack           01
		"\033[31m",   // Red             02
		"\033[32m",   // Green           03
		"\033[33m",   // Yellow          04
		"\033[34m",   // Blue            05
		"\033[35m",   // Magenta         06
		"\033[36m",   // Cyan            07
		"\033[37m",   // White           08
		"\033[90m",   // Grey            09
		"\033[91m",   // Bright Red      10
		"\033[92m",   // Bright Green    11
		"\033[93m",   // Bright Yellow   12
		"\033[94m",   // Bright Blue     13
		"\033[95m",   // Bright Magenta  14
		"\033[96m",   // Bright Cyan     15
		"\033[97m",   // Bright White    16
	};

	public:
    Renderer(int w, int h) : width(w), height(h), 
        frontBuffer(h, std::vector<Pixel>(w, { ' ', 0 })), 
        backBuffer(h, std::vector<Pixel>(w, { ' ', 0 })) {hideCursor();}

    void clearBuffer() {
        for (auto& row : backBuffer) {
            for (auto& pixel : row) {
                pixel = { ' ', 0 };  // Reset to space and default color
            }
        }
    }

    void writeToBuffer(char letter, int xpos, int ypos, int color) {
        if (xpos >= 0 && xpos < width && ypos >= 0 && ypos < height) {
            backBuffer[ypos][xpos] = { letter, color % static_cast<int>(ANSI_COLORS.size()) };  // Ensure valid color index
        }
    }

    void drawBuffer() {
        clearScreen();

        for (const auto& row : frontBuffer) {
            for (const auto& pixel : row) {
                std::cout << ANSI_COLORS[pixel.color] << pixel.letter;
            }
            std::cout << "\033[0m\n";  // Reset color and move to the next line
        }

        // Swap buffers
        frontBuffer.swap(backBuffer);
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
	