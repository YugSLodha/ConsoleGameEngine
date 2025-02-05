
#include "engine.h"

int main() {
	const int FPS = 60;
	const int width = 60;
	const int height = 20;

	Renderer renderer(width, height);
	FpsManager fps(FPS);
	Input input;
	PhysicsEngine physicsEngine;
	UI ui;

	const std::string playText = "Press <Enter> To Play";
	const std::string settingsText = "Press <Shift> To Open Settings";
	const std::string exitText = "Press <Escape> To Exit";

	clearScreen();
	while (true) {
		renderer.clearBuffer();
		float deltaTime = fps.regulate();

		renderer.drawBorder(width, height, '#', 15);

		ui.addElement(playText, width / 2 - playText.length() / 2, height / 2 + 2, 1);
		ui.addElement(settingsText, width / 2 - settingsText.length() / 2, height / 2 + 3, 1);
		ui.addElement(exitText, width / 2 - exitText.length() / 2, height / 2 + 4, 1);

		renderer.drawUI(ui);

		renderer.drawBuffer();
	}
	return 0;
}
