#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "Paddle.h"
#include "Ball.h"
#include <string>

using namespace sf;
using namespace std;

const float pi = 3.14159f;
const int gameWidth = 800;
const int gameHeight = 600;

Paddle* player = new Paddle(Vector2f(25, 100));
Paddle* enemy = new Paddle(Vector2f(25, 100));
Ball* ball = new Ball(10.0f);

bool isPlaying = false;

int speedIncrement = 20;
int playerScore = 0;
int enemyScore = 0;

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	
	RenderWindow window(VideoMode(gameWidth, gameHeight), "Pong");
	Clock clock;
	Clock AITimer;
	const Time AITime = seconds(0.1f);
	player->speed = 300;
	enemy->speed = 300;
	ball->speed = 200;

	Font font;
	if (!font.loadFromFile("calibri.ttf"))
		return EXIT_FAILURE;

	Text pauseMessage;
	pauseMessage.setFont(font);
	pauseMessage.setCharacterSize(40);
	pauseMessage.setPosition(170.f, 150.f);
	pauseMessage.setFillColor(Color::White);
	pauseMessage.setString("Welcome to SFML pong!\nPress space to start the game");

	Text playerScoreText;
	playerScoreText.setFont(font);
	playerScoreText.setCharacterSize(20);
	playerScoreText.setPosition(80.f, 80.f);
	playerScoreText.setFillColor(Color::White);
	playerScoreText.setString(to_string(playerScore));

	Text enemyScoreText;
	enemyScoreText.setFont(font);
	enemyScoreText.setCharacterSize(20);
	enemyScoreText.setPosition(700.f, 80.f);
	enemyScoreText.setFillColor(Color::White);
	enemyScoreText.setString(to_string(enemyScore));


	bool collidePlayerOnce = false;
	bool collideEnemyOnce = false;

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
		
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if ((event.type == Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
			{
				if (!isPlaying)
				{
					// (re)start the game
					isPlaying = true;
					clock.restart();

					// Reset the position of the paddles and ball
					player->paddle.setPosition(10 + player->dimensions.x / 2, gameHeight / 2);
					enemy->paddle.setPosition(gameWidth - 10 - enemy->dimensions.x / 2, gameHeight / 2);
					ball->ball.setPosition(gameWidth / 2, gameHeight / 2);

					// Reset the ball angle
					do
					{
						// Make sure the ball initial angle is not too much vertical
						ball->ballAngle = (std::rand() % 360) * 2 * pi / 360;
					} while (std::abs(std::cos(ball->ballAngle)) < 0.7f);
				}
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
			//ball->ball.getGlobalBounds().intersects(player->paddle.getGlobalBounds())
			if (ball->ball.getPosition().x - ball->radius < 0.f)
			{
				ball->ball.setPosition(400, 200);
				enemyScore++;
				enemyScoreText.setString(to_string(enemyScore));
				ball->speed = 200;
				
				if (enemyScore > 3)
				{
					isPlaying = false;
					pauseMessage.setString("You lost!\nPress space to restart or\nescape to exit");
					playerScore = 0;
					enemyScore = 0;
				}
				
			}
			if (ball->ball.getPosition().x + ball->radius > gameWidth)
			{
				ball->ball.setPosition(400, 200);
				playerScore++;
				enemyScoreText.setString(to_string(playerScore));
				ball->speed = 200;
				if (playerScore > 3)
				{
					isPlaying = false;
					pauseMessage.setString("You won!\nPress space to restart or\nescape to exit");
					playerScore = 0;
					enemyScore = 0;
				}
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
			
					
			if (ball->ball.getGlobalBounds().intersects(player->paddle.getGlobalBounds()))
			{
				if (ball->ball.getOrigin().y > player->paddle.getOrigin().y)
					ball->ballAngle = pi - ball->ballAngle + (std::rand() % 20) * pi / 100;
				else
					ball->ballAngle = pi - ball->ballAngle - (std::rand() % 20) * pi / 100;
				
				//ballSound.play();
				ball->ball.setPosition(player->paddle.getPosition().x + ball->radius + player->dimensions.x / 2 + 20.0f, ball->ball.getPosition().y);
				ball->speed += speedIncrement;
			}
		
			
			if (ball->ball.getGlobalBounds().intersects(enemy->paddle.getGlobalBounds()))
			{
				if (ball->ball.getOrigin().y > enemy->paddle.getOrigin().y)
					ball->ballAngle = pi - ball->ballAngle + (std::rand() % 20) * pi / 100;
				else
					ball->ballAngle = pi - ball->ballAngle - (std::rand() % 20) * pi / 100;

				//ballSound.play();
				ball->ball.setPosition(enemy->paddle.getPosition().x - ball->radius - enemy->dimensions.x / 2 - 20.0f, ball->ball.getPosition().y);
				ball->speed += speedIncrement;
			}
			
			collideEnemyOnce = false;
		}

		window.clear();
		if (isPlaying)
		{
			// Draw the paddles and the ball
			window.draw(playerScoreText);
			window.draw(enemyScoreText);
			window.draw(player->paddle);
			window.draw(enemy->paddle);
			window.draw(ball->ball);
			

		}

		else {
			window.draw(pauseMessage);
		}
		window.display();
	}

	return 0;
}
