#include "Ball.h"

using namespace sf;
using namespace std;
Ball::Ball(float f)
{
	radius = f;
	ball.setRadius(radius - 3);
	ball.setOutlineThickness(3);
	ball.setOutlineColor(Color::White);
	ball.setFillColor(Color::White);
	ball.setOrigin(radius / 2, radius / 2);
}

void Ball::handleBallMovement(float deltaTime)
{
	float factor = speed * deltaTime;
	ball.move(cos(ballAngle) * factor, sin(ballAngle) * factor);
}

bool Ball::handleEnemyScoreCollision()
{
	if (ball.getPosition().x - radius < 0.f)
	{
		ball.setPosition(400, 200);
		speed = 200;
		return true;
	}
	return false;
}

bool Ball::handlePlayerScoreCollision(int gameWidth)
{
	if (ball.getPosition().x + radius > gameWidth)
	{
		ball.setPosition(400, 200);
		speed = 200;
		return true;
	}
	return false;
}

void Ball::handleWallCollision(Sound* s, int gameHeight)
{
	if (ball.getPosition().y - radius < 0.f)
	{
		s->play();
		ballAngle = -ballAngle;
		ball.setPosition(ball.getPosition().x, radius + 0.1f);
	}
	if (ball.getPosition().y + radius > gameHeight)
	{
		s->play();
		ballAngle = -ballAngle;
		ball.setPosition(ball.getPosition().x, gameHeight - radius - 0.1f);
	}
}

void Ball::handlePaddleCollision(Sound* s, Paddle player, Paddle enemy)
{
	if (ball.getGlobalBounds().intersects(player.paddle.getGlobalBounds()))
	{
		Vector2f temp = ball.getPosition() - player.paddle.getPosition();
		float angle = atan2f(temp.y, temp.x) * 0.95;
		ballAngle = angle;

		s->play();
		ball.setPosition(player.paddle.getPosition().x + radius + player.dimensions.x / 2 + 20.0f, ball.getPosition().y);
		speed += 40;
	}

	if (ball.getGlobalBounds().intersects(enemy.paddle.getGlobalBounds()))
	{
		Vector2f temp = ball.getPosition() - enemy.paddle.getPosition();
		float angle = atan2f(temp.y, temp.x) * 0.95;
		ballAngle = angle;

		s->play();
		ball.setPosition(enemy.paddle.getPosition().x - radius - enemy.dimensions.x / 2 - 20.0f, ball.getPosition().y);
		speed += 40;
	}
}

Ball::~Ball()
{
}
