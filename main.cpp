#include "engine.h"
#include <iostream>
#include <memory>
#include <cmath>

int main() {
	const int width = 139;
	const int height = 30;
	const int FPS = 120;
	float deltatime;
	bool running = true;

	Camera cam;
	Renderer renderer(width, height, &cam);
	Input input;
	FPSManager fpsManager(FPS);

	UI mainMenuUi;
	// LOGO
	const int logoX = floor((width - 65) / 2);
	mainMenuUi.addElement(logoX, 3, "    ____        _ _      _   _____                           _   ", Color::Grey);
	mainMenuUi.addElement(logoX, 4, "   |  _ \\      | | |    | | |  __ \\                         | |  ", Color::Grey);
	mainMenuUi.addElement(logoX, 5, "   | |_) |_   _| | | ___| |_| |__) | __ ___  _ __ ___  _ __ | |_ ", Color::Grey);
	mainMenuUi.addElement(logoX, 6, "   |  _ <| | | | | |/ _ \\ __|  ___/ '__/ _ \\| '_ ` _ \\| '_ \\| __|", Color::Grey);
	mainMenuUi.addElement(logoX, 7, "   | |_) | |_| | | |  __/ |_| |   | | | (_) | | | | | | |_) | |_ ", Color::Grey);
	mainMenuUi.addElement(logoX, 8, "   |____/ \\__,_|_|_|\\___|\\__|_|   |_|  \\___/|_| |_| |_| .__/ \\__|", Color::Grey);
	mainMenuUi.addElement(logoX, 9, "                                                      | |        ", Color::Grey);
	mainMenuUi.addElement(logoX, 10, "                                                      |_|        ", Color::Grey);
	// Options
	mainMenuUi.addElement(floor((width - 21) / 2), 17, "Press <ENTER> to Play", Color::Red);
	mainMenuUi.addElement(floor((width - 27) / 2), 18, "Press <LSHIFT> for Settings", Color::Red);
	mainMenuUi.addElement(floor((width - 19) / 2), 19, "Press <ESC> to EXIT", Color::Red);

	auto mainMenuLoop = [&]() {
		renderer.clearBuffer();
		renderer.drawBorder('#', Color::BrightWhite);

		renderer.drawUI(mainMenuUi);

		if (input.isKeyDown(VK_ESCAPE)) {
			running = false;
		}


		renderer.drawBuffer();
		};
	auto mainMenuScreen = std::make_shared<Screen>(mainMenuLoop);
	renderer.addScreen("mainMenu", mainMenuScreen);

	renderer.setActiveScreen("mainMenu");
	clearScreen();
	hideCursor();
	while (running) {
		input.update();
		renderer.loop();
		deltatime = fpsManager.regulate();
	}
	showCursor();
	clearScreen();

	return 0;
}