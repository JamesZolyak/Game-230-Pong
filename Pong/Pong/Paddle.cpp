#include "Paddle.h"



Paddle::Paddle(Vector2f dimensions)
{
	paddle.setSize(dimensions - sf::Vector2f(3, 3));
	paddle.setOutlineThickness(3);
	paddle.setOutlineColor(sf::Color::Black);
	paddle.setFillColor(sf::Color(100, 100, 200));
	paddle.setOrigin(dimensions / 2.f);
}

void Paddle::handlePlayerMovement(float deltaTime, int gameHeight)
{
	if (Keyboard::isKeyPressed(Keyboard::Up) &&
		(paddle.getPosition().y - dimensions.y / 2 > 20.f))
	{
		paddle.move(0.f, -speed * deltaTime);
	}
	if (Keyboard::isKeyPressed(Keyboard::Down) &&
		(paddle.getPosition().y + dimensions.y / 2 < gameHeight - 20.f))
	{
		paddle.move(0.f, speed * deltaTime);
	}
}

void Paddle::handleAIMovement(float deltaTime, int gameHeight, Clock AITimer, Time AITime, Ball* ball)
{
	if (((speed < 0.f) && (paddle.getPosition().y - dimensions.y / 2 > 20.f)) ||
		((speed > 0.f) && (paddle.getPosition().y + dimensions.y / 2 < gameHeight - 20.f)))
	{
		paddle.move(0.f, speed * deltaTime);
	}

	if (AITimer.getElapsedTime() > AITime)
	{
		AITimer.restart();
		if (ball->ball.getPosition().y > paddle.getPosition().y + dimensions.y)
			speed = 250;
		else if (ball->ball.getPosition().y < paddle.getPosition().y - dimensions.y)
			speed = -250;
		else
			speed = 0.f;
	}
}
Paddle::~Paddle()
{
}
