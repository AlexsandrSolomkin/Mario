#pragma once

#include "TileTypes.h"
#include <SFML/Graphics.hpp>
#include <string>

struct Tile
{
    ETileTextureType textureType;
    ETileCollisionType collisionType;
};

struct Level
{
    std::vector<std::vector<Tile>> tiles;
    std::map<ETileTextureType, sf::Sprite> tileTextureTypeToSprite;
};

struct LevelDescriptor
{
    std::vector<std::vector<char>> levelSymbols;
    sf::Texture tileSetTexture;
    std::map<ETileTextureType, sf::IntRect> tileTextureTypeToTextureRect;
};

Level createLevel(const LevelDescriptor& LEVEL_DESCRIPTOR, sf::Vector2f tileScale);