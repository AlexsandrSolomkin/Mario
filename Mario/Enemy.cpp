#include "Enemy.h"
#include "World.h"

Enemy* createEnemy(const sf::Texture& TILE_SET_TEXTURE, sf::Vector2f drawScale, sf::Vector2f position)
{
	Enemy* enemy = new Enemy;

	const sf::Vector2i TEXTURE_TILE_SIZE(30.f, 30.f);

	enemy->sprite.setTexture(TILE_SET_TEXTURE);
	enemy->sprite.setTextureRect(sf::IntRect(122.f, 1182.f, TEXTURE_TILE_SIZE.x, TEXTURE_TILE_SIZE.y));
	enemy->sprite.setScale(drawScale);

	enemy->rect = sf::FloatRect(position.x, position.y, TEXTURE_TILE_SIZE.x * drawScale.x, TEXTURE_TILE_SIZE.y * drawScale.y);

	enemy->enemySpeed = 150.f;
	enemy->enemyVelocity.x = -enemy->enemySpeed;

	return enemy;
}

Enemy* spawnEnemyInWorld(World& world, const sf::Texture& TILE_SET_TEXTURE, sf::Vector2f drawScale, sf::Vector2f position)
{
	Enemy* enemy = createEnemy(TILE_SET_TEXTURE, drawScale, position);
	enemy->world = &world;

	world.enemies.push_back(enemy);
	world.objects.push_back(enemy);

	return enemy;
}