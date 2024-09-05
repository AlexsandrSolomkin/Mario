#pragma once

#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "GameObjectLiving.h"

enum class EPlayerDirection
{
    Left,
    Right
};

class Player : public GameObjectLiving
{
public:
    void draw(sf::RenderWindow& window) override;
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
