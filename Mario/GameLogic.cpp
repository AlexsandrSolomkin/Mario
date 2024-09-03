#include "GameLogic.h"
#include "Collisions.h"

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

void updateGame(World& world, float clockDeltaSeconds, const sf::Vector2f& TILE_SIZE)
{
    {
        for (Enemy& enemy : world.enemies)
        {
            if (!enemy.bDead)
            {
                enemy.staticObject.rect.left += enemy.enemyVelocity.x * clockDeltaSeconds;

                CollisionResult collisionResult = handleCollision(enemy.staticObject.rect, enemy.enemyVelocity, world.level, EOrientation::Horizontal, TILE_SIZE);

                if (collisionResult.bIsCollided)
                {
                    enemy.staticObject.rect.left = collisionResult.newPosition.x;
                    enemy.enemyVelocity.x *= -1;
                }
            }
        }
    }

    {
        const sf::Vector2f CACHE_PLAYER_VELOCITY = world.player.playerVelocity;
        Player& player = world.player;

        player.playerVelocity.y += world.GRAVITY * clockDeltaSeconds;

        {
            player.staticObject.rect.left += player.playerVelocity.x * clockDeltaSeconds;
            CollisionResult collisionResult = handleCollision(player.staticObject.rect, player.playerVelocity, world.level, EOrientation::Horizontal, TILE_SIZE);

            if (collisionResult.bIsCollided)
            {
                player.staticObject.rect.left = collisionResult.newPosition.x;
            }
        }

        {
            world.player.staticObject.rect.top -= world.player.playerVelocity.y * clockDeltaSeconds;
            CollisionResult collisionResult = handleCollision(world.player.staticObject.rect, world.player.playerVelocity, world.level, EOrientation::Vertical, TILE_SIZE);

            if (collisionResult.bIsCollided)
            {
                player.staticObject.rect.top = collisionResult.newPosition.y;

                if (player.playerVelocity.y < 0)
                {
                    player.bIsPlayerOnGround = true;
                    player.playerVelocity.x = 0.f;
                }

                player.playerVelocity.y = 0.f;
            }
        }

        world.player.playerVelocity.x = 0.f;

        for (Enemy& enemy : world.enemies)
        {
            if (!enemy.bDead)
            {
                if (world.player.staticObject.rect.intersects(enemy.staticObject.rect))
                {
                    if ((!player.bIsPlayerOnGround) && (player.staticObject.rect.top < enemy.staticObject.rect.top) && (CACHE_PLAYER_VELOCITY.y < 0))
                    {
                        enemy.bDead = true;
                        player.playerVelocity.y = player.playerKillEnemyJumpSpeed;
                        player.score += 50;
                    }
                    else
                    {
                        world.bGameOver = true;
                        break;
                    }
                }
            }
        }

        for (Coin& coin : world.coins)
        {
            if (!coin.bDead)
            {
                if (world.player.staticObject.rect.intersects(coin.staticObject.rect))
                {
                    if ((!player.bIsPlayerOnGround) && (player.staticObject.rect.top < coin.staticObject.rect.top) && (CACHE_PLAYER_VELOCITY.y < 0))
                    {
                        coin.bDead = true;
                        player.score += 100;
                    }
                }
            }
        }
    }
}

void drawGame(sf::RenderWindow& window, World& world, const sf::Vector2f TILE_SIZE)
{
    window.clear();

    for (int i = 0; i < world.level.tiles.size(); ++i)
        for (int j = 0; j < world.level.tiles[i].size(); ++j)
        {
            const Tile& tile = world.level.tiles[i][j];

            sf::Sprite& sprite = world.level.tileTextureTypeToSprite[tile.textureType];
            sprite.setPosition(TILE_SIZE.x * j, TILE_SIZE.y * i);
            window.draw(sprite);
        }

    for (Enemy& enemy : world.enemies)
    {
        if (!enemy.bDead)
        {
            enemy.staticObject.sprite.setPosition(enemy.staticObject.rect.left, enemy.staticObject.rect.top);
            window.draw(enemy.staticObject.sprite);
        }
    }

    for (Coin& coin : world.coins)
    {
        if (!coin.bDead)
        {
            coin.staticObject.sprite.setPosition(coin.staticObject.rect.left, coin.staticObject.rect.top);
            window.draw(coin.staticObject.sprite);
        }
    }

    world.player.staticObject.sprite.setPosition(world.player.staticObject.rect.left, world.player.staticObject.rect.top);
    window.draw(world.player.staticObject.sprite);

    world.scoreText.setString("SCORE: " + std::to_string(world.player.score));
    window.draw(world.scoreText);

    window.display();
}
