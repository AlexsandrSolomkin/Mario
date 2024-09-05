#pragma once

#include "GameObjectLiving.h"

class Coin : public GameObjectLiving
{
public:
};

Coin* createCoin(const sf::Texture& TILE_SET_TEXTURE, sf::Vector2f drawScale, sf::Vector2f position);
Coin* spawnCoinInWorld(World& world, const sf::Texture& TILE_SET_TEXTURE, sf::Vector2f drawScale, sf::Vector2f position);