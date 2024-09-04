#pragma once

#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "GameObject.h"

enum class EPlayerDirection
{
    Left,
    Right
};

class Player : public GameObjectLivin
{
public:
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