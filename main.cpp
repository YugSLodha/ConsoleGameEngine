#include "engine.h"
#include <cmath>

int main() {
	int width = 139;
	int height = 30;
	// TODO: MAKE DYNAMIC WIDTH AND HEIGHT
	const int FPS = 60;
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
	ui.addElement(floor((width - 21) / 2), 16, "Press <ENTER> to Play", Color::Yellow);
	ui.addElement(floor((width - 23) / 2), 17, "Press <ESCAPE> to  Quit", Color::Yellow);

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

	UI gameUI;
	gameUI.addElement(1, 1, "P1: 00", Color::BrightRed);
	gameUI.addElement(width - 7, 1, "P2: 00", Color::BrightRed);

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

		ballX += ballSpeedX * deltatime;
		ballY += ballSpeedY * deltatime;

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
			}
		}
		if (ballX + ballWidth >= width - 10 && ballX <= width - 10 + 1) {
			if (ballY + ballHeight >= ySpritePosP2 && ballY <= ySpritePosP2 + paddleHeight) {
				ballX = width - 10 - ballWidth - 1;
				ballSpeedX *= -1;
			}
		}

		if (ballX < 0) {
			ballX = width / 2.0f;
			ballY = height / 2.0f;
			ballSpeedX *= -1;
			p2Score++;
			renderer.drawSprite(ball, Position(static_cast<int>(width / 2.0f), static_cast<int>(height / 2.0f)));
		}
		if (ballX > width) {
			ballX = width / 2.0f;
			ballY = height / 2.0f;
			ballSpeedX *= -1;
			p1Score++;
			renderer.drawSprite(ball, Position(static_cast<int>(width / 2.0f), static_cast<int>(height / 2.0f)));
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
