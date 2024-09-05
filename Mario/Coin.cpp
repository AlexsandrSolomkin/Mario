#include "Coin.h"
#include "World.h"

Coin* createCoin(const sf::Texture& TILE_SET_TEXTURE, sf::Vector2f drawScale, sf::Vector2f position)
{
	Coin* coin = new Coin;

	const sf::Vector2i COIN_TEXTURES_SIZE(21, 30);

	coin->sprite.setTexture(TILE_SET_TEXTURE);
	coin->sprite.setTextureRect(sf::IntRect(98.f, 97.f, COIN_TEXTURES_SIZE.x, COIN_TEXTURES_SIZE.y));
	coin->sprite.setScale(drawScale);

	coin->rect = sf::FloatRect(position.x, position.y, COIN_TEXTURES_SIZE.x * drawScale.x, COIN_TEXTURES_SIZE.y * drawScale.y);

	return coin;
}

Coin* spawnCoinInWorld(World& world, const sf::Texture& TILE_SET_TEXTURE, sf::Vector2f drawScale, sf::Vector2f position)
{
	Coin* coin = createCoin(TILE_SET_TEXTURE, drawScale, position);
	coin->world = &world;

	world.coins.push_back(coin);
	world.objects.push_back(coin);

	return coin;
}