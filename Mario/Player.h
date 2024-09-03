#pragma once

#include <SFML/Graphics.hpp>
#include "StaticObject.h"

struct Player
{
    StaticObject staticObject;
    sf::Vector2f playerVelocity;
    float playerSpeed = 200.f;
    float playerJumpSpeed = 520.f;
    float playerKillEnemyJumpSpeed = 200.f;
    bool bIsPlayerOnGround = false;
    int score = 0;
};