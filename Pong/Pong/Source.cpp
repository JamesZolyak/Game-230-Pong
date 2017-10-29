#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "Paddle.h"
#include "Ball.h"

using namespace sf;

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	const float pi = 3.14159f;
	const int gameWidth = 800;
	const int gameHeight = 600;
	RenderWindow window(VideoMode(gameWidth, gameHeight), "Pong");
	Paddle* player = new Paddle(Vector2f(25, 100));
	player->position = Vector2f(20, 60);
	player->speed = 400;

	Paddle* enemy = new Paddle(Vector2f(25, 100));
	enemy->position = Vector2f(400, 100);
	enemy->speed = 400;

	Ball* ball = new Ball(10.0f);
	ball->position = Vector2f(200, 200);
	bool isPlaying = false;
	Clock clock;
	Clock AITimer;
	const Time AITime = seconds(0.1f);
	while (window.isOpen())
	{
		if (!isPlaying)
		{
			player->paddle.setPosition(10 + player->dimensions.x / 2, gameHeight / 2);
			enemy->paddle.setPosition(gameWidth - 10 - enemy->dimensions.x / 2, gameHeight / 2);
			ball->ball.setPosition(gameWidth / 2, gameHeight / 2);
		}
		isPlaying = true;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed)
			{

			}
		}

		if(isPlaying)
		{
			float deltaTime = clock.restart().asSeconds();
			if (Keyboard::isKeyPressed(Keyboard::Up) &&
				(player->position.y - player->dimensions.y / 2 > 5.f))
			{
				player->paddle.move(0.f, -player->speed * deltaTime);
			}
			if (Keyboard::isKeyPressed(Keyboard::Down) &&
				(player->position.y + player->dimensions.y / 2 < gameHeight - 5.f))
			{
				player->paddle.move(0.f, player->speed * deltaTime);
			}

			if (((enemy->speed < 0.f) && (enemy->position.y - enemy->dimensions.y / 2 > 5.f)) ||
				((enemy->speed > 0.f) && (enemy->position.y + enemy->dimensions.y / 2 < gameHeight - 5.f)))
			{
				enemy->paddle.move(0.f, enemy->speed * deltaTime);
			}

			if (AITimer.getElapsedTime() > AITime)
			{
				AITimer.restart();
				if (ball->position.y + ball->radius > enemy->position.y + enemy->dimensions.y / 2)
					enemy->speed = 400;
				else if (ball->position.y - ball->radius < enemy->position.y - enemy->dimensions.y / 2)
					enemy->speed = -400;
				else
					enemy->speed = 0.f;
			}

			float factor = ball->speed * deltaTime;
			ball->ball.move(std::cos(ball->ballAngle) * factor, std::sin(ball->ballAngle) * factor);

			// Check collisions between the ball and the screen
			if (ball->position.x - ball->radius < 0.f)
			{
				isPlaying = false;
				//pauseMessage.setString("You lost!\nPress space to restart or\nescape to exit");
			}
			if (ball->position.x + ball->radius > gameWidth)
			{
				isPlaying = false;
				//pauseMessage.setString("You won!\nPress space to restart or\nescape to exit");
			}
			if (ball->position.y - ball->radius < 0.f)
			{
				//ballSound.play();
				ball->ballAngle = -ball->ballAngle;
				ball->ball.setPosition(ball->position.x, ball->radius + 0.1f);
				//ball->position.x = ball->radius + 0.1f;
			}
			if (ball->position.y + ball->radius > gameHeight)
			{
				//ballSound.play();
				ball->ballAngle = -ball->ballAngle;
				ball->ball.setPosition(ball->position.x, gameHeight - ball->radius - 0.1f);
			}

			// Check the collisions between the ball and the paddles
			// Left Paddle
			if (ball->position.x - ball->radius < player->position.x + player->dimensions.x / 2 &&
				ball->position.x - ball->radius > player->position.x &&
				ball->position.y + ball->radius >= player->position.y - player->dimensions.y / 2 &&
				ball->position.y - ball->radius <= player->position.y + player->dimensions.y / 2)
			{
				if (ball->position.y > player->position.y)
					ball->ballAngle = pi - ball->ballAngle + (std::rand() % 20) * pi / 180;
				else
					ball->ballAngle = pi - ball->ballAngle - (std::rand() % 20) * pi / 180;

				//ballSound.play();
				ball->ball.setPosition(player->position.x + ball->radius + player->dimensions.x / 2 + 0.1f, ball->position.y);
			}

			// Right Paddle
			if (ball->position.x + ball->radius > enemy->position.x - enemy->dimensions.x / 2 &&
				ball->position.x + ball->radius < enemy->position.x &&
				ball->position.y + ball->radius >= enemy->position.y - enemy->dimensions.y / 2 &&
				ball->position.y - ball->radius <= enemy->position.y + enemy->dimensions.y / 2)
			{
				if (ball->position.y > enemy->position.y)
					ball->ballAngle = pi - ball->ballAngle + (std::rand() % 20) * pi / 180;
				else
					ball->ballAngle = pi - ball->ballAngle - (std::rand() % 20) * pi / 180;

				//ballSound.play();
				ball->ball.setPosition(enemy->position.x - ball->radius - enemy->dimensions.x / 2 - 0.1f, ball->position.y);
			}
		}


		if (isPlaying)
		{
			// Draw the paddles and the ball
			window.clear();
			window.draw(player->paddle);
			window.draw(enemy->paddle);
			window.draw(ball->ball);
			window.display();

		}
	}

	return 0;
}
