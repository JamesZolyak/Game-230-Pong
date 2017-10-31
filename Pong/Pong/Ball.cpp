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

Ball::~Ball()
{
}
