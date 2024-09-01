#pragma once

#include "Player.h"
#include "Collisions.h"

void handleInput(Player& player);

void handleWindowEvent(const sf::Event& EVENT, Player& player);

void updateGame(Player& player, const Level& LEVEL, float gravity, float clockDeltaSeconds, const sf::Vector2f& TILE_SIZE);

void drawGame(sf::RenderWindow& window, Level& level, Player& player, const sf::Vector2f TILE_SIZE);
