#include "GameLogic.h"

void handleInput(Player& player)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        player.playerVelocity.x = -player.playerSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        player.playerVelocity.x = player.playerSpeed;
    }
}

void handleWindowEvent(const sf::Event& EVENT, Player& player)
{
    if (EVENT.type == sf::Event::KeyPressed)
    {
        if (EVENT.key.code == sf::Keyboard::Space)
        {
            if (player.bIsPlayerOnGround)
            {
                player.bIsPlayerOnGround = false;
                player.playerVelocity.y = player.playerJumpSpeed;
            }
        }
    }
}

void updateGame(Player& player, const Level& LEVEL, float gravity, float clockDeltaSeconds, const sf::Vector2f& TILE_SIZE)
{
    player.playerVelocity.y += gravity * clockDeltaSeconds;

    {
        player.playerRect.left += player.playerVelocity.x * clockDeltaSeconds;
        CollisionResult collisionResult = handleCollision(player.playerRect, player.playerVelocity, LEVEL, EOrientation::Horizontal, TILE_SIZE);

        if (collisionResult.bIsCollided)
        {
            player.playerRect.left = collisionResult.newPosition.x;
        }
    }

    {
        player.playerRect.top -= player.playerVelocity.y * clockDeltaSeconds;
        CollisionResult collisionResult = handleCollision(player.playerRect, player.playerVelocity, LEVEL, EOrientation::Vertical, TILE_SIZE);

        if (collisionResult.bIsCollided)
        {
            player.playerRect.top = collisionResult.newPosition.y;

            if (player.playerVelocity.y < 0)
            {
                player.bIsPlayerOnGround = true;
                player.playerVelocity.x = 0.f;
            }
        }
    }

    player.playerVelocity.x = 0.f;
}

void drawGame(sf::RenderWindow& window, Level& level, Player& player, const sf::Vector2f TILE_SIZE)
{
    window.clear();

    for (int i = 0; i < level.tiles.size(); ++i)
        for (int j = 0; j < level.tiles[i].size(); ++j)
        {
            const Tile& tile = level.tiles[i][j];

            sf::Sprite& sprite = level.tileTextureTypeToSprite[tile.textureType];
            sprite.setPosition(TILE_SIZE.x * j, TILE_SIZE.y * i);
            window.draw(sprite);
        }

    player.playerSprite.setPosition(player.playerRect.left, player.playerRect.top);
    window.draw(player.playerSprite);
    window.display();
}
