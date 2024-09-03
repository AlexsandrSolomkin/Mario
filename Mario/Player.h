#pragma once

#include <SFML/Graphics.hpp>
#include "StaticObject.h"
#include "Animation.h"

enum class EPlayerDirection
{
    Left,
    Right
};

struct Player
{
    StaticObject staticObject;
    sf::Vector2f playerVelocity;
    float playerSpeed = 200.f;
    float playerJumpSpeed = 520.f;
    float playerKillEnemyJumpSpeed = 200.f;
    bool bIsPlayerOnGround = false;
    int score = 0;

    Animation idleAnimation;
    Animation walkAnimation;
    Animation jumpAnimation;

    Animation* currentAnimation = nullptr;

    EPlayerDirection playerAnimationDirection = EPlayerDirection::Right;
};