#include "Ball.h"

using namespace sf;

Ball::Ball(float f)
{
	radius = f;
	ball.setRadius(radius - 3);
	ball.setOutlineThickness(3);
	ball.setOutlineColor(Color::Black);
	ball.setFillColor(Color::White);
	ball.setOrigin(radius / 2, radius / 2);
}


Ball::~Ball()
{
}
