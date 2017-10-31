#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace sf;

class Ball
{
public:
	CircleShape ball;
	Vector2f position;
	float radius;
	float ballAngle;
	int speed;
	Ball(float f);

	void handleBallMovement(float deltaTime);
	~Ball();
};

