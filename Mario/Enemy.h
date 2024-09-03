#pragma once

#include <SFML/Graphics.hpp>
#include "StaticObject.h"

struct Enemy
{
    StaticObject staticObject;
    sf::Vector2f enemyVelocity;
    float enemySpeed = 200.f;
    bool bDead = false;
};
