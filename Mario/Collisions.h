#pragma once

#include "Level.h"

enum class EOrientation
{
    Horizontal,
    Vertical
};

struct CollisionResult
{
    bool bIsCollided = false;
    sf::Vector2f newPosition;
};

CollisionResult handleCollision(
    const sf::FloatRect& OBJECT_RECT,
    const sf::Vector2f& OBJECT_VELOCITY,
    const Level& LEVEL,
    EOrientation orientation,
    const sf::Vector2f& TILE_SIZE);