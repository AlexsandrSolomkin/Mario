#include "GameLogic.h"

int main()
{
    const sf::Vector2i TEXTURE_TILE_SIZE(30.f, 30.f);
    const sf::Vector2f DRAW_SCALE(3.f, 3.f);
    const sf::Vector2f TILE_SIZE(TEXTURE_TILE_SIZE.x * DRAW_SCALE.x, TEXTURE_TILE_SIZE.y * DRAW_SCALE.y);
    const sf::Vector2f GROUND_SPRITE_SIZE(30.f, 30.f);
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mario");

    sf::Texture tileSetTexture;
    tileSetTexture.loadFromFile("Resources/MarioTextures_v1.png");
    sf::Texture tileSetTexture2;
    tileSetTexture2.loadFromFile("Resources/MarioTextures_v2.png");

    LevelDescriptor levelDescriptor;
    levelDescriptor.levelSymbols =
    {
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'B', 'S', 'S', 'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'}
    };

    levelDescriptor.tileSetTexture = tileSetTexture;
    levelDescriptor.tileTextureTypeToTextureRect[ETileTextureType::Empty] = sf::IntRect(1460, 788, TEXTURE_TILE_SIZE.x, TEXTURE_TILE_SIZE.y);
    levelDescriptor.tileTextureTypeToTextureRect[ETileTextureType::Ground] = sf::IntRect(363, 531, TEXTURE_TILE_SIZE.x, TEXTURE_TILE_SIZE.y);

	sf::Font font;
	font.loadFromFile("Resources/Fonts/Roboto-Bold.ttf");

	while (window.isOpen())
	{
		World world;

		world.renderWindow = &window;

		world.scoreText.setFont(font);
		world.scoreText.setCharacterSize(36);
		world.scoreText.setPosition(300.f, 50.f);
		world.scoreText.setFillColor(sf::Color::White);

		world.level = createLevel(world, levelDescriptor, DRAW_SCALE, TILE_SIZE);

		{
			world.player = new Player;
			world.player->world = &world;
			world.objects.push_back(world.player);

			world.player->sprite.setTexture(tileSetTexture);
			world.player->sprite.setTextureRect(sf::IntRect(121, 49, TEXTURE_TILE_SIZE.x, TEXTURE_TILE_SIZE.y));
			world.player->sprite.setScale(DRAW_SCALE);

			world.player->sprite.setOrigin(TEXTURE_TILE_SIZE.x / 2.f, TEXTURE_TILE_SIZE.y / 2.f);

			world.player->rect = sf::FloatRect(100.f, 400.f, 20 * DRAW_SCALE.x, 34 * DRAW_SCALE.y);

			{
				world.player->walkAnimation.texture = tileSetTexture;
				world.player->walkAnimation.speed = 40.f;
				world.player->walkAnimation.frames.push_back(sf::IntRect(7, 52, 23, 32));
				world.player->walkAnimation.frames.push_back(sf::IntRect(36, 52, 24, 32));
				world.player->walkAnimation.frames.push_back(sf::IntRect(65, 52, 25, 30));
				world.player->walkAnimation.frames.push_back(sf::IntRect(94, 52, 25, 30));
				world.player->walkAnimation.frames.push_back(sf::IntRect(122, 52, 26, 31));
				world.player->walkAnimation.frames.push_back(sf::IntRect(152, 52, 23, 32));
				world.player->walkAnimation.frames.push_back(sf::IntRect(178, 52, 22, 32));
				world.player->walkAnimation.frames.push_back(sf::IntRect(203, 52, 19, 32));
				world.player->walkAnimation.frames.push_back(sf::IntRect(229, 52, 17, 33));
				world.player->walkAnimation.frames.push_back(sf::IntRect(250, 52, 15, 33));
				world.player->walkAnimation.frames.push_back(sf::IntRect(272, 52, 15, 33));
				world.player->walkAnimation.frames.push_back(sf::IntRect(293, 52, 16, 35));
				world.player->walkAnimation.frames.push_back(sf::IntRect(316, 52, 17, 35));
				world.player->walkAnimation.frames.push_back(sf::IntRect(338, 52, 19, 34));
				world.player->walkAnimation.frames.push_back(sf::IntRect(363, 52, 23, 31));
				world.player->walkAnimation.frames.push_back(sf::IntRect(388, 52, 24, 30));
				world.player->walkAnimation.frames.push_back(sf::IntRect(415, 52, 24, 30));
				world.player->walkAnimation.frames.push_back(sf::IntRect(441, 52, 24, 30));
				world.player->walkAnimation.frames.push_back(sf::IntRect(468, 52, 25, 31));
				world.player->walkAnimation.frames.push_back(sf::IntRect(495, 52, 24, 33));
				world.player->walkAnimation.frames.push_back(sf::IntRect(522, 52, 24, 33));
				world.player->walkAnimation.frames.push_back(sf::IntRect(548, 52, 24, 34));
				world.player->walkAnimation.frames.push_back(sf::IntRect(574, 52, 22, 34));
				world.player->walkAnimation.frames.push_back(sf::IntRect(599, 52, 19, 34));
				world.player->walkAnimation.frames.push_back(sf::IntRect(621, 52, 23, 31));
			}
			{
				world.player->idleAnimation.texture = tileSetTexture;
				world.player->idleAnimation.speed = 0.f;
				world.player->idleAnimation.frames.push_back(sf::IntRect(7, 10, 16, 32));
			}
			{
				world.player->jumpAnimation.texture = tileSetTexture;
				world.player->jumpAnimation.speed = 20.f;
				world.player->jumpAnimation.frames.push_back(sf::IntRect(682, 103, 23, 18));
				world.player->jumpAnimation.frames.push_back(sf::IntRect(713, 103, 24, 19));
				world.player->jumpAnimation.frames.push_back(sf::IntRect(744, 101, 22, 22));
				world.player->jumpAnimation.frames.push_back(sf::IntRect(775, 101, 16, 22));
				world.player->jumpAnimation.frames.push_back(sf::IntRect(800, 98, 20, 24));
				world.player->jumpAnimation.frames.push_back(sf::IntRect(829, 100, 22, 18));
				world.player->jumpAnimation.frames.push_back(sf::IntRect(857, 98, 23, 19));
				world.player->jumpAnimation.frames.push_back(sf::IntRect(886, 96, 21, 22));
				world.player->jumpAnimation.frames.push_back(sf::IntRect(914, 97, 15, 22));
				world.player->jumpAnimation.frames.push_back(sf::IntRect(937, 97, 21, 24));
			}
		}
		spawnEnemyInWorld(world, tileSetTexture2, DRAW_SCALE, sf::Vector2f(300.f, 450.f));
		spawnEnemyInWorld(world, tileSetTexture2, DRAW_SCALE, sf::Vector2f(400.f, 450.f));

		spawnCoinInWorld(world, tileSetTexture2, DRAW_SCALE, sf::Vector2f(300.f, 450.f));
		spawnCoinInWorld(world, tileSetTexture2, DRAW_SCALE, sf::Vector2f(465.f, 360.f));

		sf::Clock clock;

		while (!world.bGameOver && window.isOpen())
		{
			sf::sleep(sf::seconds(0.001f));

			float clockDeltaSeconds = clock.getElapsedTime().asSeconds();
			clock.restart();

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}

				handleWindowEvent(event, *world.player);

				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::P)
					{
						world.bGameOver = true;
					}
				}
			}

			handleInput(*world.player);

			updateGame(world, clockDeltaSeconds, TILE_SIZE);

			drawGame(window, world, TILE_SIZE);
		}
	}

    return 0;
}