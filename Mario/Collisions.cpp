#include "Collisions.h"

CollisionResult handleCollision(
    const sf::FloatRect& OBJECT_RECT,
    const sf::Vector2f& OBJECT_VELOCITY,
    const Level& LEVEL,
    EOrientation orientation,
    const sf::Vector2f& TILE_SIZE)
{
    const int X_LEFT_TILE = OBJECT_RECT.left / TILE_SIZE.x;
    int xRightTile = (OBJECT_RECT.left + OBJECT_RECT.width - 1) / TILE_SIZE.x;

    const int Y_TOP_TILE = OBJECT_RECT.top / TILE_SIZE.y;
    int yBottomTile = (OBJECT_RECT.top + OBJECT_RECT.height - 1) / TILE_SIZE.y;

    CollisionResult result;
    result.newPosition = OBJECT_RECT.getPosition();

    for (int i = Y_TOP_TILE; i <= yBottomTile; ++i)
    {
        for (int j = X_LEFT_TILE; j <= xRightTile; ++j)
        {
            const Tile& TILE = LEVEL.tiles[i][j];
            const sf::Vector2f TILE_POSITION = sf::Vector2f(j * TILE_SIZE.x, i * TILE_SIZE.y);

            if (TILE.collisionType == ETileCollisionType::Collision)
            {
                result.bIsCollided = true;

                if (orientation == EOrientation::Horizontal)
                {
                    if (OBJECT_VELOCITY.x < 0)
                    {
                        result.newPosition = sf::Vector2f(TILE_POSITION.x + TILE_SIZE.x, OBJECT_RECT.top);
                    }
                    else
                    {
                        result.newPosition = sf::Vector2f(TILE_POSITION.x - OBJECT_RECT.width, OBJECT_RECT.top);
                    }
                }
                else
                {
                    if (OBJECT_VELOCITY.y < 0)
                    {
                        result.newPosition = sf::Vector2f(OBJECT_RECT.left, TILE_POSITION.y - OBJECT_RECT.height);
                    }
                    else
                    {
                        result.newPosition = sf::Vector2f(OBJECT_RECT.left, TILE_POSITION.y + TILE_SIZE.y);
                    }
                }
                return result;
            }
        }
    }
    return result;
}