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
	//player->position = Vector2f(20, 60);
	player->speed = 400;

	Paddle* enemy = new Paddle(Vector2f(25, 100));
	//enemy->position = Vector2f(400, 100);
	enemy->speed = 400;

	Ball* ball = new Ball(10.0f);
	//ball->position = Vector2f(200, 200);
	ball->speed = 100.0f;
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
			//ball->ball.setPosition(gameWidth / 2, gameHeight / 2);
			ball->ball.setPosition(400, 200);
			do
			{
				// Make sure the ball initial angle is not too much vertical
				ball->ballAngle = (std::rand() % 360) * 2 * pi / 360;
			} while (std::abs(std::cos(ball->ballAngle)) < 0.7f);
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
				(player->paddle.getPosition().y - player->dimensions.y / 2 > 20.f))
			{
				player->paddle.move(0.f, -player->speed * deltaTime);
			}
			if (Keyboard::isKeyPressed(Keyboard::Down) &&
				(player->paddle.getPosition().y + player->dimensions.y / 2 < gameHeight - 20.f))
			{
				player->paddle.move(0.f, player->speed * deltaTime);
			}

			if (((enemy->speed < 0.f) && (enemy->paddle.getPosition().y - enemy->dimensions.y / 2 > 20.f)) ||
				((enemy->speed > 0.f) && (enemy->paddle.getPosition().y + enemy->dimensions.y / 2 < gameHeight - 20.f)))
			{
				enemy->paddle.move(0.f, enemy->speed * deltaTime);
			}
			
			if (AITimer.getElapsedTime() > AITime)
			{
				AITimer.restart();
				if (ball->ball.getPosition().y > enemy->paddle.getPosition().y )
					enemy->speed = 400;
				else if (ball->ball.getPosition().y < enemy->paddle.getPosition().y  )
					enemy->speed = -400;
				else
					enemy->speed = 0.f;
			}

			float factor = ball->speed * deltaTime;
			ball->ball.move(std::cos(ball->ballAngle) * factor, std::sin(ball->ballAngle) * factor);

			// Check collisions between the ball and the screen
			if (ball->ball.getPosition().x - ball->radius < 0.f)
			{
				//isPlaying = false;
				//pauseMessage.setString("You lost!\nPress space to restart or\nescape to exit");
			}
			if (ball->ball.getPosition().x + ball->radius > gameWidth)
			{
				//isPlaying = false;
				//pauseMessage.setString("You won!\nPress space to restart or\nescape to exit");
			}
			if (ball->ball.getPosition().y - ball->radius < 0.f)
			{
				//ballSound.play();
				ball->ballAngle = -ball->ballAngle;
				ball->ball.setPosition(ball->ball.getPosition().x, ball->radius + 0.1f);
			}
			if (ball->ball.getPosition().y + ball->radius > gameHeight)
			{
				//ballSound.play();
				ball->ballAngle = -ball->ballAngle;
				ball->ball.setPosition(ball->ball.getPosition().x, gameHeight - ball->radius - 0.1f);
			}
			
				
			// Check the collisions between the ball and the paddles
			// Left Paddle

			if (ball->ball.getGlobalBounds().intersects(player->paddle.getGlobalBounds()))
			{
				if (ball->ball.getOrigin().y > player->paddle.getOrigin().y)
					ball->ballAngle = pi - ball->ballAngle + (std::rand() % 20) * pi / 180;
				else
					ball->ballAngle = pi - ball->ballAngle - (std::rand() % 20) * pi / 180;
				
				//ballSound.play();
				//ball->ball.setPosition(player->paddle.getPosition().x + ball->radius + player->dimensions.x / 2 + 10.0f, ball->ball.getPosition().y);
				
			}

			if (ball->ball.getGlobalBounds().intersects(enemy->paddle.getGlobalBounds()))
			{
				if (ball->ball.getOrigin().y > enemy->paddle.getOrigin().y)
					ball->ballAngle = pi - ball->ballAngle + (std::rand() % 20) * pi / 180;
				else
					ball->ballAngle = pi - ball->ballAngle - (std::rand() % 20) * pi / 180;

				//ballSound.play();
				//ball->ball.setPosition(enemy->paddle.getPosition().x - ball->radius - enemy->dimensions.x / 2 - 0.5f, ball->ball.getPosition().y);
			}
			//if (ball->ball.getPosition().x - ball->radius < player->paddle.getPosition().x + player->dimensions.x / 2 &&
			//	ball->ball.getPosition().x - ball->radius > player->paddle.getPosition().x &&
			//	ball->ball.getPosition().y + ball->radius >= player->paddle.getPosition().y - player->dimensions.y / 2 &&
			//	ball->ball.getPosition().y - ball->radius <= player->paddle.getPosition().y + player->dimensions.y / 2)
			//{
			//	if (ball->ball.getPosition().y > player->paddle.getPosition().y)
			//		ball->ballAngle = pi - ball->ballAngle + (std::rand() % 20) * pi / 180;
			//	else
			//		ball->ballAngle = pi - ball->ballAngle - (std::rand() % 20) * pi / 180;

			//	//ballSound.play();
			//	ball->ball.setPosition(player->paddle.getPosition().x + ball->radius + player->dimensions.x / 2 + 0.1f, ball->ball.getPosition().y);
			//}

			// Right Paddle
			//if (ball->ball.getPosition().x + ball->radius > enemy->paddle.getPosition().x - enemy->dimensions.x / 2 &&
			//	ball->ball.getPosition().x + ball->radius < enemy->paddle.getPosition().x &&
			//	ball->ball.getPosition().y + ball->radius >= enemy->paddle.getPosition().y - enemy->dimensions.y / 2 &&
			//	ball->ball.getPosition().y - ball->radius <= enemy->paddle.getPosition().y + enemy->dimensions.y / 2)
			//{
			//	if (ball->ball.getPosition().y > enemy->paddle.getPosition().y)
			//		ball->ballAngle = pi - ball->ballAngle + (std::rand() % 20) * pi / 180;
			//	else
			//		ball->ballAngle = pi - ball->ballAngle - (std::rand() % 20) * pi / 180;

			//	//ballSound.play();
			//	ball->ball.setPosition(enemy->paddle.getPosition().x - ball->radius - enemy->dimensions.x / 2 - 0.1f, ball->ball.getPosition().y);
			//}
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
