#pragma once

#include <SFML/Graphics.hpp>

struct Player
{
    sf::Vector2f playerSize;
    sf::Sprite playerSprite;
    sf::FloatRect playerRect;
    sf::Vector2f playerVelocity;
    float playerSpeed = 200.f;
    float playerJumpSpeed = 400.f;
    bool bIsPlayerOnGround = false;
};