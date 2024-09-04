#pragma once

#include "Level.h"
#include "Player.h"
#include "Enemy.h"
#include "Coin.h"
#include "Camera.h"

struct World
{
	Player player;
	std::vector<Enemy*> enemies;
	std::vector<Coin*> coins;
	std::vector<GameObjectLivin*> objects;
	Level level;

	Camera camera;

	const float GRAVITY = -700.f;

	bool bGameOver = false;

	sf::Text scoreText;

	sf::RenderWindow* renderWindow = nullptr;
};