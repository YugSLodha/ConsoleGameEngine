#include "BitBox.hpp"
#include <iostream>
#include <memory>
#include <cmath>

// Gameplay Settings
std::string difficulty = "MEDIUM";        // EASY / MEDIUM / HARD
int playerColor = Color::Red;
char playerCharacter = 'O';             // '@', '#', 'A', etc.
bool autoFire = false;
// Visual Settings
bool screenShake = true;
char borderStyle = '#';                 // Could be '#', '*', '-', etc.
bool hitFlash = true;
// Audio Settings
bool soundEnabled = true;
int volume = 100;                       // 0 - 100
bool musicEnabled = false;
std::vector<DataItem> settingsData;

std::vector<DataItem> getCurrentSettings() {
	return {
		{"difficulty", difficulty},
		{"playerColor", playerColor}, // already an int
		{"playerCharacter", std::string(1, playerCharacter)},
		{"autoFire", autoFire},
		{"screenShake", screenShake},
		{"borderStyle", std::string(1, borderStyle)},
		{"hitFlash", hitFlash},
		{"soundEnabled", soundEnabled},
		{"volume", volume},
		{"musicEnabled", musicEnabled}
	};
}


void applySettings(const std::vector<DataItem>& data) {
	for (const auto& item : data) {
		if (item.name == "difficulty") difficulty = std::get<std::string>(item.value);
		else if (item.name == "playerColor") playerColor = std::get<int>(item.value); // no conversion needed
		else if (item.name == "playerCharacter") playerCharacter = std::get<std::string>(item.value)[0];
		else if (item.name == "autoFire") autoFire = std::get<bool>(item.value);
		else if (item.name == "screenShake") screenShake = std::get<bool>(item.value);
		else if (item.name == "borderStyle") borderStyle = std::get<std::string>(item.value)[0];
		else if (item.name == "hitFlash") hitFlash = std::get<bool>(item.value);
		else if (item.name == "soundEnabled") soundEnabled = std::get<bool>(item.value);
		else if (item.name == "volume") volume = std::get<int>(item.value);
		else if (item.name == "musicEnabled") musicEnabled = std::get<bool>(item.value);
	}
}



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

	settingsData = loadData("save/settings.json");
	applySettings(settingsData);

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

		if (input.isKeyDown(VK_LSHIFT)) {
			renderer.setActiveScreen("settings");
		}

		renderer.drawBuffer();
		};
	auto mainMenuScreen = std::make_shared<Screen>(mainMenuLoop);
	renderer.addScreen("mainMenu", mainMenuScreen);

	UI settingsUi;
	settingsUi.addElement(5, 4, "Settings Menu", Color::BrightWhite);
	settingsUi.addElement(5, 12, "Press <BACKSPACE> to go back", Color::Red);

	auto settingsLoop = [&]() {
		renderer.clearBuffer();
		renderer.drawBorder('#', Color::BrightWhite);

		renderer.drawUI(settingsUi);

		if (input.isKeyDown(VK_BACK)) {
			settingsData = getCurrentSettings();
			saveData(settingsData, "save/settings.json");
			renderer.setActiveScreen("mainMenu");
		}

		renderer.drawBuffer();
		};
	auto settingsScreen = std::make_shared<Screen>(settingsLoop);
	renderer.addScreen("settings", settingsScreen);

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