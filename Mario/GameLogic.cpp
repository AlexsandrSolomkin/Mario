#include "GameLogic.h"
#include "Collisions.h"
#include <algorithm>

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
        for (Enemy* enemy : world.enemies)
        {
            if (!enemy->bDead)
            {
                enemy->rect.left += enemy->enemyVelocity.x * clockDeltaSeconds;

                CollisionResult collisionResult = handleCollision(enemy->rect, enemy->enemyVelocity, world.level, EOrientation::Horizontal, TILE_SIZE);

                if (collisionResult.bIsCollided)
                {
                    enemy->rect.left = collisionResult.newPosition.x;
                    enemy->enemyVelocity.x *= -1;
                }
            }
        }
    }

    {
        const sf::Vector2f CACHE_PLAYER_VELOCITY = world.player.playerVelocity;
        Player& player = world.player;

        player.playerVelocity.y += world.GRAVITY * clockDeltaSeconds;

        {
            player.rect.left += player.playerVelocity.x * clockDeltaSeconds;
            CollisionResult collisionResult = handleCollision(player.rect, player.playerVelocity, world.level, EOrientation::Horizontal, TILE_SIZE);

            if (collisionResult.bIsCollided)
            {
                player.rect.left = collisionResult.newPosition.x;
            }
        }

        {
            player.rect.top -= player.playerVelocity.y * clockDeltaSeconds;
            CollisionResult collisionResult = handleCollision(player.rect, player.playerVelocity, world.level, EOrientation::Vertical, TILE_SIZE);

            if (collisionResult.bIsCollided)
            {
                player.rect.top = collisionResult.newPosition.y;

                if (player.playerVelocity.y < 0)
                {
                    player.bIsPlayerOnGround = true;
                }

                player.playerVelocity.y = 0.f;
            }
        }

        {
            if (world.renderWindow != nullptr)
            {
                world.camera.position.x = player.rect.getPosition().x;
                world.camera.position.x -= world.renderWindow->getSize().x / 2 - player.rect.width / 2.f;

                const float MIN_CAMERA_X = 0.f;
                const float MAX_CAMERA_X = world.level.tiles[0].size() * TILE_SIZE.x - world.renderWindow->getSize().x;

                world.camera.position.x = std::clamp(world.camera.position.x, MIN_CAMERA_X, MAX_CAMERA_X);
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

        for (Enemy* enemy : world.enemies)
        {
            if (!enemy->bDead)
            {
                if (world.player.rect.intersects(enemy->rect))
                {
                    if ((!player.bIsPlayerOnGround) && (player.rect.top < enemy->rect.top) && (CACHE_PLAYER_VELOCITY.y < 0))
                    {
                        enemy->bDead = true;
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

        for (Coin* coin : world.coins)
        {
            if (!coin->bDead)
            {
                if (world.player.rect.intersects(coin->rect))
                {
                    if ((!player.bIsPlayerOnGround) && (player.rect.top < coin->rect.top) && (CACHE_PLAYER_VELOCITY.y < 0))
                    {
                        coin->bDead = true;
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
    {
        for (int j = 0; j < world.level.tiles[i].size(); ++j)
        {
            const Tile& tile = world.level.tiles[i][j];

            sf::Vector2f tileDrawPosition = { TILE_SIZE.x * j, TILE_SIZE.y * i };
            tileDrawPosition -= world.camera.position;

            sf::Sprite& sprite = world.level.tileTextureTypeToSprite[tile.textureType];
            sprite.setPosition(tileDrawPosition);
            window.draw(sprite);
        }
    }

    for (GameObjectLivin* object : world.objects)
    {
        if (!object->bDead)
        {
            sf::Vector2f drawPosition = object->rect.getPosition();
            drawPosition -= world.camera.position;
            object->sprite.setPosition(drawPosition);
            window.draw(object->sprite);
        }
    }

    Player& player = world.player;

    {
        if (player.currentAnimation != nullptr)
        {
            player.sprite.setTextureRect(player.currentAnimation->getCurrentFrame());
        }

        const int SCALE_X_SIGN = player.playerAnimationDirection == EPlayerDirection::Left ? -1 : 1;
        const float SCALE_X = std::abs(player.sprite.getScale().x) * SCALE_X_SIGN;
        player.sprite.setScale(SCALE_X, player.sprite.getScale().y);

        sf::Vector2f drawPosition = 
        { 
            player.rect.left + player.rect.width / 2, 
            player.rect.top + player.rect.height / 2 
        };

        drawPosition -= world.camera.position;
        player.sprite.setPosition(drawPosition);

    }
    
    window.draw(player.sprite);

    world.scoreText.setString("SCORE: " + std::to_string(player.score));
    window.draw(world.scoreText);

    window.display();
}
