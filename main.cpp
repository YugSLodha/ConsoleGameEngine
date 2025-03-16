#include "engine.h"
#include <cmath>

int main() {
	int width = 139;
	int height = 30;
	// TODO: MAKE DYNAMIC WIDTH AND HEIGHT
	const int FPS = 60;
	float deltatime;
	bool running = true;

	Camera camera;
	Renderer renderer(width, height, &camera);
	Input input;
	FPSManager fpsManager(FPS);

	UI ui;
	ui.addElement(floor((width - 4) / 2), 5, "PONG", Color::Grey);
	ui.addElement(floor((width - 21) / 2), 16, "Press <ENTER> to Play", Color::Yellow);
	ui.addElement(floor((width - 23) / 2), 17, "Press <ESCAPE> to  Quit", Color::Yellow);

	auto gsLoop = [&]() {
		renderer.clearBuffer();
		renderer.drawBorder('#', Color::BrightBlue);
		if (input.isKeyPressed(VK_ESCAPE)) {
			running = false;
		}
		renderer.drawBuffer();
		};

	auto gameScreen = std::make_shared<Screen>(gsLoop);
	renderer.addScreen("gameScreen", gameScreen);

	auto mmLoop = [&]() {
		renderer.clearBuffer();
		renderer.drawBorder('#', Color::BrightWhite);
		renderer.drawUI(ui);
		if (input.isKeyPressed(VK_ESCAPE)) {
			running = false;
		}
		if (input.isKeyPressed(VK_RETURN)) {
			renderer.setActiveScreen("gameScreen");
		}
		renderer.drawBuffer();
		};

	auto mainMenu = std::make_shared<Screen>(mmLoop);
	renderer.addScreen("mainMenu", mainMenu);

	renderer.setActiveScreen("mainMenu");
	clearScreen();
	while (running) {
		input.update();
		renderer.loop();
		deltatime = fpsManager.regulate();
	}

	showCursor();
	clearScreen();

	return 0;
}
