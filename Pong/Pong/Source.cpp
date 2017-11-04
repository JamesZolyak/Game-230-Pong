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
#include <cmath>

using namespace sf;
using namespace std;

const float pi = 3.14159f;
const int gameWidth = 800;
const int gameHeight = 600;

Paddle* player = new Paddle(Vector2f(25, 100));
Paddle* enemy = new Paddle(Vector2f(25, 100));
Ball* ball = new Ball(10.0f);

bool isPlaying = false;

int speedIncrement = 40;
int playerScore = 0;
int enemyScore = 0;

bool isSinglePlayer = true;

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	
	RenderWindow window(VideoMode(gameWidth, gameHeight), "Pong");
	Clock clock;
	Clock AITimer;
	const Time AITime = seconds(0.1f);
	player->speed = 250;
	enemy->speed = 250;
	ball->speed = 200;

	Font font;
	if (!font.loadFromFile("calibri.ttf"))
		return EXIT_FAILURE;

	Text pauseMessage;
	pauseMessage.setFont(font);
	pauseMessage.setCharacterSize(40);
	pauseMessage.setPosition(170.f, 150.f);
	pauseMessage.setFillColor(Color::White);
	pauseMessage.setString("Welcome to SFML pong!\nPress 1 for single player\nPress 2 for multiplayer");

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

	SoundBuffer sb;
	sb.loadFromFile("Bounce.wav");
	Sound s;
	s.setBuffer(sb);

	Texture backgroundImage;
	backgroundImage.loadFromFile("230-PongBackground.png");
	RectangleShape background;
	background.setSize(Vector2f(gameWidth, gameHeight));
	background.setTexture(&backgroundImage);
	while (window.isOpen())
	{
		if (!isPlaying)
		{
			player->paddle.setPosition(10 + player->dimensions.x / 2, gameHeight / 2);
			enemy->paddle.setPosition(gameWidth - 10 - enemy->dimensions.x / 2, gameHeight / 2);
			ball->ball.setPosition(gameWidth / 2, gameHeight / 2);
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

			if ((event.type == Event::KeyPressed) && (event.key.code == sf::Keyboard::Num1))
			{

				isSinglePlayer = true;
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
			if ((event.type == Event::KeyPressed) && (event.key.code == sf::Keyboard::Num2))
			{
				isSinglePlayer = false;
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

			player->handlePlayerMovement(deltaTime, gameHeight);
			if (isSinglePlayer)
			{
				enemy->handleAIMovement(deltaTime, gameHeight, AITimer, AITime, ball);
			}
			else
			{
				enemy->handleSecondPlayerMovement(deltaTime, gameHeight);
			}
			ball->handleBallMovement(deltaTime);


			// Check collisions between the ball and the screen

			if (ball->ball.getPosition().x - ball->radius < 0.f)
			{
				ball->ball.setPosition(400, 200);
				enemyScore++;
				enemyScoreText.setString(to_string(enemyScore));
				ball->speed = 200;
				
				if (enemyScore >= 5)
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
				playerScoreText.setString(to_string(playerScore));
				ball->speed = 200;
				if (playerScore >= 5)
				{
					isPlaying = false;
					pauseMessage.setString("You won!\nPress space to restart or\nescape to exit");
					playerScore = 0;
					enemyScore = 0;
				}
			}
			if (ball->ball.getPosition().y - ball->radius < 0.f)
			{
				s.play();
				ball->ballAngle = -ball->ballAngle;
				ball->ball.setPosition(ball->ball.getPosition().x, ball->radius + 0.1f);
			}
			if (ball->ball.getPosition().y + ball->radius > gameHeight)
			{
				s.play();
				ball->ballAngle = -ball->ballAngle;
				ball->ball.setPosition(ball->ball.getPosition().x, gameHeight - ball->radius - 0.1f);
			}
			
			if (ball->ball.getGlobalBounds().intersects(player->paddle.getGlobalBounds()))
			{
				
				Vector2f temp = ball->ball.getPosition() - player->paddle.getPosition();
				float angle = atan2f(temp.y, temp.x) * 0.95;
				ball->ballAngle = angle;

				s.play();
				ball->ball.setPosition(player->paddle.getPosition().x + ball->radius + player->dimensions.x / 2 + 20.0f, ball->ball.getPosition().y);
				ball->speed += speedIncrement;
			}
		
			
			if (ball->ball.getGlobalBounds().intersects(enemy->paddle.getGlobalBounds()))
			{

				Vector2f temp = ball->ball.getPosition() - enemy->paddle.getPosition();
				float angle = atan2f(temp.y, temp.x) * 0.95;
				ball->ballAngle = angle;

				s.play();
				ball->ball.setPosition(enemy->paddle.getPosition().x - ball->radius - enemy->dimensions.x / 2 - 20.0f, ball->ball.getPosition().y);
				ball->speed += speedIncrement;
			}
			
			
		}

		window.clear();
		window.draw(background);
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
