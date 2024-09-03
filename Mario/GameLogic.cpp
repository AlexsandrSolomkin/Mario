#include "GameLogic.h"
#include "Collisions.h"

void handleInput(Player& player)
{
    player.playerVelocity.x = 0.f;

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
            player.staticObject.rect.top -= player.playerVelocity.y * clockDeltaSeconds;
            CollisionResult collisionResult = handleCollision(player.staticObject.rect, player.playerVelocity, world.level, EOrientation::Vertical, TILE_SIZE);

            if (collisionResult.bIsCollided)
            {
                player.staticObject.rect.top = collisionResult.newPosition.y;

                if (player.playerVelocity.y < 0)
                {
                    player.bIsPlayerOnGround = true;
                    //player.playerVelocity.x = 0.f;
                }

                player.playerVelocity.y = 0.f;
            }
        }

        if (std::abs(player.playerVelocity.x) > 0.0001f)
        {
            player.playerAnimationDirection = player.playerVelocity.x < 0 ? EPlayerDirection::Left : EPlayerDirection::Right;
        }

        if (!player.bIsPlayerOnGround)
        {
            player.currentAnimation = &player.jumpAnimation;
        }
        else if (std::abs(player.playerVelocity.x) > 0.0001f)
        {
            player.currentAnimation = &player.walkAnimation;
        }
        else
        {
            player.currentAnimation = &player.idleAnimation;
        }

        player.currentAnimation->update(clockDeltaSeconds);

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

    Player& player = world.player;

    {
        if (player.currentAnimation != nullptr)
        {
            player.staticObject.sprite.setTextureRect(player.currentAnimation->getCurrentFrame());
        }

        const int SCALE_X_SIGN = player.playerAnimationDirection == EPlayerDirection::Left ? -1 : 1;
        const float SCALE_X = std::abs(player.staticObject.sprite.getScale().x) * SCALE_X_SIGN;
        player.staticObject.sprite.setScale(SCALE_X, player.staticObject.sprite.getScale().y);

        const float PLAYER_DRAW_LEFT_POS = player.staticObject.rect.left + player.staticObject.rect.width / 2;
        const float PLAYER_DRAW_TOP_POS = player.staticObject.rect.top + player.staticObject.rect.height / 2;

        player.staticObject.sprite.setPosition(PLAYER_DRAW_LEFT_POS, PLAYER_DRAW_TOP_POS);

    }
    
    window.draw(player.staticObject.sprite);

    world.scoreText.setString("SCORE: " + std::to_string(player.score));
    window.draw(world.scoreText);

    window.display();
}
