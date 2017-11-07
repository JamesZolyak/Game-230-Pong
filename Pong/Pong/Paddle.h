#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace sf;

class Paddle
{
public:
	RectangleShape paddle;
	Vector2f position;
	Vector2f dimensions;
	int speed;

	Paddle(Vector2f dimensions);
	void handlePlayerMovement(float deltaTime, int gameHeight);
	void handleSecondPlayerMovement(float deltaTime, int gameHeight);
	void handleAIMovement(float deltaTime, int gameHeight, Clock AITimer, Time AITime, float ballYPosition);
	~Paddle();
};

