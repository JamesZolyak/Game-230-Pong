#include "Paddle.h"



Paddle::Paddle(Vector2f dimensions)
{
	paddle.setSize(dimensions - sf::Vector2f(3, 3));
	paddle.setOutlineThickness(3);
	paddle.setOutlineColor(sf::Color::Black);
	paddle.setFillColor(sf::Color(100, 100, 200));
	paddle.setOrigin(dimensions / 2.f);
}


Paddle::~Paddle()
{
}
