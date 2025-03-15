#include "engine.h"
#include <cmath>

int main() {
	const int width = 139;
	const int height = 31;
	const int FPS = 60;
	float deltatime;
	bool running = true;

	Camera camera;
	Renderer renderer(width, height, &camera);
	Input input;
	FPSManager fpsManager(FPS);

	UI ui;
	ui.addElement(floor((width - 11) / 2), 5, "Flappy Bird", Color::Yellow);
	ui.addElement(floor((width - 21) / 2), 16, "Press <ENTER> to Play", Color::Yellow);
	ui.addElement(floor((width - 23) / 2), 17, "Press <ESCAPE> to  Quit", Color::Yellow);

	auto mmRender = [&]() {
		renderer.clearBuffer();
		renderer.drawBorder('#', Color::BrightWhite);
		renderer.drawUI(ui);
		renderer.drawBuffer();
		};

	auto mmUpdate = [&]() {
		if (input.isKeyPressed(VK_ESCAPE)) {
			running = false;
		}
		if (input.isKeyPressed(VK_RETURN)) {
			exit(0);
		}
		};

	auto mainMenu = std::make_shared<Screen>(mmUpdate, mmRender);
	renderer.addScreen("mainMenu", mainMenu);

	renderer.setActiveScreen("mainMenu");
	clearScreen();
	while (running) {
		input.update();
		renderer.update();
		renderer.render();
		deltatime = fpsManager.regulate();
	}

	showCursor();
	clearScreen();

	return 0;
}
