#include "engine.h"
#include <cmath>

int main() {
	int width = 139;
	int height = 30;
	// TODO: MAKE DYNAMIC WIDTH AND HEIGHT
	const int FPS = 120;
	float deltatime;
	bool running = true;
	const std::vector<std::vector<char>> spriteText = { {' ', '_', ' '},
													   {'|', ' ', '|'},
													   {'|', ' ', '|'},
													   {'|', ' ', '|'},
													   {'|', '_', '|'}, };

	const std::vector<std::vector<char>> ballText = { {' ', '_',' '},{'|','_','|'} };

	Camera camera;
	Renderer renderer(width, height, &camera);
	Input input;
	FPSManager fpsManager(FPS);

	UI ui;
	ui.addElement(floor((width - 4) / 2), 5, "PONG", Color::Grey);
	ui.addElement(floor((width - 15) / 2), 6, "Read Help First", Color::Grey);
	ui.addElement(floor((width - 21) / 2), 16, "Press <ENTER> to Play", Color::Yellow);
	ui.addElement(floor((width - 23) / 2), 17, "Press <ESCAPE> to  Quit", Color::Yellow);
	ui.addElement(floor((width - 18) / 2), 18, "Press <H> for Help", Color::Yellow);

	float ySpritePosP1 = 1.0f;
	float ySpritePosP2 = 1.0f;

	int p1Score = 0;
	int p2Score = 0;

	Sprite paddle;
	paddle.setTextureSolidCol(spriteText, Color::Grey);

	Sprite ball;
	ball.setTextureSolidCol(ballText, Color::Yellow);
	float ballSpeedX = -30.0f; // units per second (adjust as needed)
	float ballSpeedY = 15.0f; // units per second (adjust as needed)
	float ballX = width / 2.0f;
	float ballY = height / 2.0f;
	int ballWidth = ballText.size();
	int ballHeight = ballText[0].size();
	float speedMultiplier = 1.0f + 0.1f * std::max(p1Score, p2Score);

	UI gameUI;
	gameUI.addElement(1, 1, "P1: 00", Color::BrightRed);
	gameUI.addElement(width - 7, 1, "P2: 00", Color::BrightRed);

	UI helpUI;
	helpUI.addElement((width - 10) / 2, 3, "HELP", Color::BrightCyan);
	helpUI.addElement(5, 6, "W / S         - Move P1 Paddle", Color::Yellow);
	helpUI.addElement(5, 7, "Arrow Up/Down - Move P2 Paddle", Color::Yellow);
	helpUI.addElement(5, 8, "<ENTER>       - Start Game", Color::Yellow);
	helpUI.addElement(5, 9, "<ESCAPE>      - Quit / Back", Color::Yellow);
	helpUI.addElement(5, 10, "First To 5 Wins!", Color::Yellow);

	UI gameOverUI;
	std::string winnerText = "Player X Wins!";
	gameOverUI.addElement((width - winnerText.size()) / 2, height / 2 - 1, winnerText, Color::BrightGreen);
	gameOverUI.addElement((width - 20) / 2, height / 2 + 1, "Press <E> for Menu", Color::Yellow);

	auto gameOverLoop = [&]() {
		renderer.clearBuffer();
		renderer.drawBorder('#', Color::BrightWhite);
		renderer.drawUI(gameOverUI);
		if (input.isKeyPressed('E')) {
			// Reset scores
			p1Score = 0;
			p2Score = 0;
			// Reset ball
			ballX = width / 2.0f;
			ballY = height / 2.0f;
			ballSpeedX = -30.0f;
			// Return to menu
			renderer.setActiveScreen("mainMenu");
		}
		renderer.drawBuffer();
		};
	auto gameOverScreen = std::make_shared<Screen>(gameOverLoop);
	renderer.addScreen("gameOver", gameOverScreen);

	auto helpLoop = [&]() {
		renderer.clearBuffer();
		renderer.drawBorder('#', Color::BrightWhite);
		renderer.drawUI(helpUI);

		if (input.isKeyDown(VK_ESCAPE)) {
			renderer.setActiveScreen("mainMenu");
			Beep(750, 150);
		}
		renderer.drawBuffer();
		};

	auto helpScreen = std::make_shared<Screen>(helpLoop);
	renderer.addScreen("helpScreen", helpScreen);

	auto gsLoop = [&]() {
		renderer.clearBuffer();
		renderer.drawBorder('#', Color::BrightWhite);


		const float paddleSpeed = 25.0f; // units per second (adjust as needed)

		if (input.isKeyDown('S')) {
			ySpritePosP1 += paddleSpeed * deltatime;
		}
		else if (input.isKeyDown('W')) {
			ySpritePosP1 -= paddleSpeed * deltatime;
		}
		if (input.isKeyDown(VK_DOWN)) {
			ySpritePosP2 += paddleSpeed * deltatime;
		}
		else if (input.isKeyDown(VK_UP)) {
			ySpritePosP2 -= paddleSpeed * deltatime;
		}

		int paddleHeight = spriteText.size();
		int maxY = height - paddleHeight - 2;
		if (ySpritePosP1 < 1) ySpritePosP1 = 1;
		if (ySpritePosP1 > maxY) ySpritePosP1 = maxY;
		if (ySpritePosP2 < 1) ySpritePosP2 = 1;
		if (ySpritePosP2 > maxY) ySpritePosP2 = maxY;

		ballX += ballSpeedX * deltatime * speedMultiplier;
		ballY += ballSpeedY * deltatime * speedMultiplier;

		if (ballY <= 1) {
			ballY = 1;
			ballSpeedY *= -1;
		}
		if (ballY >= height - ballHeight) {
			ballY = height - ballHeight;
			ballSpeedY *= -1;
		}
		if (ballX <= 7 + 1 && ballX + ballWidth >= 7) {
			if (ballY + ballHeight >= ySpritePosP1 && ballY <= ySpritePosP1 + paddleHeight) {
				ballX = 7 + 2;
				ballSpeedX *= -1;
				Beep(800, 75);
			}
		}
		if (ballX + ballWidth >= width - 10 && ballX <= width - 10 + 1) {
			if (ballY + ballHeight >= ySpritePosP2 && ballY <= ySpritePosP2 + paddleHeight) {
				ballX = width - 10 - ballWidth - 1;
				ballSpeedX *= -1;
				Beep(800, 75);
			}
		}

		if (ballX < 0) {
			ballX = width / 2.0f;
			ballY = height / 2.0f;
			ballSpeedX *= -1;
			p2Score++;
			Beep(400, 150);
			renderer.drawSprite(ball, Position(static_cast<int>(width / 2.0f), static_cast<int>(height / 2.0f)));
		}
		if (ballX > width) {
			ballX = width / 2.0f;
			ballY = height / 2.0f;
			ballSpeedX *= -1;
			p1Score++;
			Beep(400, 150);
			renderer.drawSprite(ball, Position(static_cast<int>(width / 2.0f), static_cast<int>(height / 2.0f)));
		}

		if (p1Score >= 5 || p2Score >= 5) {
			std::string winText = p1Score >= 5 ? "Player 1 Wins!" : "Player 2 Wins!";
			gameOverUI.updateElement(0, winText, Color::BrightGreen);
			Beep(1000, 200); // Victory beep
			renderer.setActiveScreen("gameOver");
			return;
		}

		char bufferP1[10], bufferP2[10];
		snprintf(bufferP1, sizeof(bufferP1), "P1: %02d", p1Score);
		snprintf(bufferP2, sizeof(bufferP2), "P2: %02d", p2Score);

		gameUI.updateElement(0, bufferP1, Color::BrightRed);
		gameUI.updateElement(1, bufferP2, Color::BrightRed);

		renderer.drawUI(gameUI);
		renderer.drawSprite(ball, Position(static_cast<int>(ballX), static_cast<int>(ballY)));
		renderer.drawSprite(paddle, Position(7, static_cast<int>(ySpritePosP1)));
		renderer.drawSprite(paddle, Position(width - 10, static_cast<int>(ySpritePosP2)));

		if (input.isKeyDown(VK_ESCAPE)) {
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
		if (input.isKeyDown(VK_ESCAPE)) {
			running = false;
			Beep(200, 150);
		}
		if (input.isKeyDown(VK_RETURN)) {
			renderer.setActiveScreen("gameScreen");
			Beep(1000, 100);
		}
		if (input.isKeyDown('H')) {
			renderer.setActiveScreen("helpScreen");
			Beep(750, 150);
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
