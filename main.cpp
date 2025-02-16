
#include "engine.h"

int main() {
	const int width = 100;
	const int height = 30;
	const int fps = 120;
	float deltatime;

	Title gameName({ 'h', 'e', 'l', 'l', 'o' }, 1, 1, 2);
	UI MainMenu;
	Renderer renderer(width, height);
	FpsManager fpsManager(fps);

	MainMenu.addTitle(gameName);

	while (true) {
		renderer.clearBuffer();

		MainMenu.draw();

		deltatime = fpsManager.regulate();
		renderer.drawBuffer();
	}

};