#include "Level.h"
#include "World.h"

void Tile::draw(sf::RenderWindow& window)
{
    Level& level = world->level;
    sf::Vector2f tileSize = world->level.tileSize;
    sf::Vector2f tileDrawPosition = { tileSize.x * tilePosition.x, tileSize.y * tilePosition.y };
    tileDrawPosition -= world->camera.position;

    sf::Sprite& sprite = world->level.tileTextureTypeToSprite[textureType];
    sprite.setPosition(tileDrawPosition);
    window.draw(sprite);
}

Level createLevel(World& world, const LevelDescriptor& LEVEL_DESCRIPTOR, sf::Vector2f tileScale, sf::Vector2f tileSize)
{
    Level level;
    level.tileSize = tileSize;

    for (int i = 0; i < LEVEL_DESCRIPTOR.levelSymbols.size(); ++i)
    {
        level.tiles.push_back({});
        for (int j = 0; j < LEVEL_DESCRIPTOR.levelSymbols[i].size(); ++j)
        {
            Tile* tile = new Tile;
            tile->world = &world;
            tile->tilePosition.x = j;
            tile->tilePosition.y = i;

            if (LEVEL_DESCRIPTOR.levelSymbols[i][j] == 'B')
            {
                tile->textureType = ETileTextureType::Ground;
                tile->collisionType = ETileCollisionType::Collision;
            }
            if (LEVEL_DESCRIPTOR.levelSymbols[i][j] == 'S')
            {
                tile->textureType = ETileTextureType::Empty;
                tile->collisionType = ETileCollisionType::NoCollision;
            }

            world.objects.push_back(tile);
            level.tiles[i].push_back(tile);
        }
    }

    for (std::map<ETileTextureType, sf::IntRect>::const_iterator it = LEVEL_DESCRIPTOR.tileTextureTypeToTextureRect.begin();
        it != LEVEL_DESCRIPTOR.tileTextureTypeToTextureRect.end(); ++it)
    {
        sf::Sprite sprite;
        sprite.setTexture(LEVEL_DESCRIPTOR.tileSetTexture);
        sprite.setTextureRect(it->second);
        sprite.setScale(tileScale);
        level.tileTextureTypeToSprite[it->first] = sprite;
    }

    return level;
}