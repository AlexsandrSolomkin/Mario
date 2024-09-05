#pragma once

#include <SFML/Graphics.hpp>
#include "GameObjectLiving.h"


class Enemy : public GameObjectLiving
{
public:
    sf::Vector2f enemyVelocity;
    float enemySpeed = 200.f;
};

Enemy* createEnemy(const sf::Texture& TILE_SET_TEXTURE, sf::Vector2f drawScale, sf::Vector2f position);
Enemy* spawnEnemyInWorld(World& world, const sf::Texture& TILE_SET_TEXTURE, sf::Vector2f drawScale, sf::Vector2f position);