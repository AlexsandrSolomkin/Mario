#include "Player.h"
#include "World.h"

void Player::draw(sf::RenderWindow& window)
{
    if (currentAnimation != nullptr)
    {
        sprite.setTextureRect(currentAnimation->getCurrentFrame());
    }

    const int SCALE_X_SIGN = playerAnimationDirection == EPlayerDirection::Left ? -1 : 1;
    const float SCALE_X = std::abs(sprite.getScale().x) * SCALE_X_SIGN;
    sprite.setScale(SCALE_X, sprite.getScale().y);

    sf::Vector2f drawPosition = { rect.left + rect.width / 2, rect.top + rect.height / 2 };

    drawPosition -= world->camera.position;
    sprite.setPosition(drawPosition);
    window.draw(sprite);
}