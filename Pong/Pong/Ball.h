#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "Paddle.h"

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
	bool handleEnemyScoreCollision();
	bool handlePlayerScoreCollision(int gameWidth);
	void handleWallCollision(Sound s, int gameHeight);
	void handlePaddleCollision(Sound s, Paddle player, Paddle enemy);
	~Ball();
};

