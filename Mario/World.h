#pragma once

#include "Level.h"
#include "Player.h"
#include "Enemy.h"
#include "Coin.h"

struct World
{
	Player player;
	std::vector<Enemy> enemies;
	std::vector<Coin> coins;
	Level level;

	const float GRAVITY = -700.f;

	bool bGameOver = false;

	sf::Text scoreText;
};