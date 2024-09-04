#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"


class Enemy : public GameObjectLivin
{
public:
    sf::Vector2f enemyVelocity;
    float enemySpeed = 200.f;
};
