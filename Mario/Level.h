#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include "TileTypes.h"
#include "GameObject.h"

class Tile : public GameObject
{
public:
    void draw(sf::RenderWindow& window) override;

public:
    ETileTextureType textureType;
    ETileCollisionType collisionType;

    sf::Vector2i tilePosition;
};

struct Level
{
    std::vector<std::vector<Tile*>> tiles;
    std::map<ETileTextureType, sf::Sprite> tileTextureTypeToSprite;

    sf::Vector2f tileSize;
};

struct LevelDescriptor
{
    std::vector<std::vector<char>> levelSymbols;
    sf::Texture tileSetTexture;
    std::map<ETileTextureType, sf::IntRect> tileTextureTypeToTextureRect;
};

Level createLevel(World& world, const LevelDescriptor& LEVEL_DESCRIPTOR, sf::Vector2f tileScale, sf::Vector2f tileSize);
