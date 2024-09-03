#pragma once

#include "World.h"

void handleInput(Player& player);

void handleWindowEvent(const sf::Event& EVENT, Player& player);

void updateGame(World& world, float clockDeltaSeconds, const sf::Vector2f& TILE_SIZE);

void drawGame(sf::RenderWindow& window, World& world, const sf::Vector2f TILE_SIZE);
