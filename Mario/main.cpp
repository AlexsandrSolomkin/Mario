#include "GameLogic.h"

int main()
{
    const sf::Vector2i TEXTURE_TILE_SIZE(30.f, 30.f);
    const sf::Vector2f DRAW_SCALE(3.f, 3.f);
    const sf::Vector2f TILE_SIZE(TEXTURE_TILE_SIZE.x * DRAW_SCALE.x, TEXTURE_TILE_SIZE.y * DRAW_SCALE.y);
    const float GRAVITY = -600.f;
    const sf::Vector2f GROUND_SPRITE_SIZE(30.f, 30.f);
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mario");

    sf::Texture tileSetTexture;
    tileSetTexture.loadFromFile("Resources/MarioTextures_v1.png");

    LevelDescriptor levelDescriptor;
    levelDescriptor.levelSymbols =
    {
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'S', 'B', 'S', 'B', 'S', 'S', 'B'},
        {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'}
    };

    levelDescriptor.tileSetTexture = tileSetTexture;
    levelDescriptor.tileTextureTypeToTextureRect[ETileTextureType::Empty] = sf::IntRect(1460, 788, TEXTURE_TILE_SIZE.x, TEXTURE_TILE_SIZE.y);
    levelDescriptor.tileTextureTypeToTextureRect[ETileTextureType::Ground] = sf::IntRect(363, 531, TEXTURE_TILE_SIZE.x, TEXTURE_TILE_SIZE.y);

    Level level = createLevel(levelDescriptor, DRAW_SCALE);

    Player player;
    player.playerSprite.setTexture(tileSetTexture);
    player.playerSprite.setTextureRect(sf::IntRect(121, 49, TEXTURE_TILE_SIZE.x, TEXTURE_TILE_SIZE.y));
    player.playerSprite.setScale(DRAW_SCALE);

    player.playerRect = sf::FloatRect(100.f, 400.f, TEXTURE_TILE_SIZE.x * DRAW_SCALE.x, TEXTURE_TILE_SIZE.y * DRAW_SCALE.y);

    sf::Clock clock;

    while (window.isOpen())
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

            handleWindowEvent(event, player);
        }

        handleInput(player);

        updateGame(player, level, GRAVITY, clockDeltaSeconds, TILE_SIZE);

        drawGame(window, level, player, TILE_SIZE);
    }

    return 0;
}