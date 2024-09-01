#include "Level.h"

Level createLevel(const LevelDescriptor& LEVEL_DESCRIPTOR, sf::Vector2f tileScale)
{
    Level level;

    for (int i = 0; i < LEVEL_DESCRIPTOR.levelSymbols.size(); ++i)
    {
        level.tiles.push_back({});
        for (int j = 0; j < LEVEL_DESCRIPTOR.levelSymbols[i].size(); ++j)
        {
            Tile tile;
            if (LEVEL_DESCRIPTOR.levelSymbols[i][j] == 'B')
            {
                tile.textureType = ETileTextureType::Ground;
                tile.collisionType = ETileCollisionType::Collision;
            }
            if (LEVEL_DESCRIPTOR.levelSymbols[i][j] == 'S')
            {
                tile.textureType = ETileTextureType::Empty;
                tile.collisionType = ETileCollisionType::NoCollision;
            }

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