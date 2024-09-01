#include <SFML/Graphics.hpp>
#include <string>

enum class ETileTextureType
{
    Empty,
    Ground
};

enum class ETileCollisionType
{
    NoCollision,
    Collision
};

struct Tile
{
    ETileTextureType textureType;
    ETileCollisionType collisionType;
};

struct Level
{
    std::vector<std::vector<Tile>> tiles;
};

struct LevelDescriptor
{
    std::vector<std::vector<char>> levelSymbols;
};

Level createLevel(const LevelDescriptor& LEVEL_DESCRIPTOR)
{
    Level level;

    for (int i = 0; i < LEVEL_DESCRIPTOR.levelSymbols.size(); ++i)
    {
        level.tiles.push_back({});
        for (int j = 0; j < LEVEL_DESCRIPTOR.levelSymbols[i].size(); ++j)
        {
            Tile tile;
            if (LEVEL_DESCRIPTOR.levelSymbols[i][j] == 'B')
            {
                tile.textureType = ETileTextureType::Ground;
                tile.collisionType = ETileCollisionType::Collision;
            }
            if (LEVEL_DESCRIPTOR.levelSymbols[i][j] == 'S')
            {
                tile.textureType = ETileTextureType::Empty;
                tile.collisionType = ETileCollisionType::NoCollision;
            }

            level.tiles[i].push_back(tile);
        }
    }

    return level;
}

enum class EOrientation
{
    Horizontal,
    Vertical
};

sf::Vector2f handleCollision(
    const sf::FloatRect& OBJECT_RECT, 
    const sf::Vector2f& OBJECT_VELOCITY,
    const Level& LEVEL, 
    EOrientation orientation,
    const sf::Vector2f& TILE_SIZE)
{
    const int X_LEFT_TILE = OBJECT_RECT.left / TILE_SIZE.x;
    const int X_RIGHT_TILE = (OBJECT_RECT.left + OBJECT_RECT.width) / TILE_SIZE.x;

    const int Y_TOP_TILE = OBJECT_RECT.top / TILE_SIZE.y;
    const int Y_BOTTOM_TILE = (OBJECT_RECT.top + OBJECT_RECT.height) / TILE_SIZE.y;

    for (int i = Y_TOP_TILE; i < Y_BOTTOM_TILE; ++i)
    {
        for (int j = X_LEFT_TILE; j <= X_RIGHT_TILE; ++j)
        {
            const Tile& TILE = LEVEL.tiles[i][j];
            const sf::Vector2f TILE_POSITION = sf::Vector2f(j * TILE_SIZE.x, i * TILE_SIZE.y);

            if (TILE.collisionType == ETileCollisionType::Collision)
            {
                if (orientation == EOrientation::Horizontal)
                {
                    if (OBJECT_VELOCITY.x < 0)
                    {
                        return sf::Vector2f(TILE_POSITION.x + TILE_SIZE.x, OBJECT_RECT.top);
                    }
                    else
                    {
                        return sf::Vector2f(TILE_POSITION.x - OBJECT_RECT.width, OBJECT_RECT.top);
                    }
                }
            }
        }
    }

    return OBJECT_RECT.getPosition();
}

int main()
{
    LevelDescriptor levelDescriptor;
    levelDescriptor.levelSymbols =
    {
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'S', 'S', 'B', 'S', 'S', 'S', 'S', 'B'},
        {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'}
    };

    Level level = createLevel(levelDescriptor);

    const sf::Vector2f TEXTURE_TILE_SIZE(30.f, 30.f);
    const sf::Vector2f DRAW_SCALE(3.f, 3.f);
    const sf::Vector2f TILE_SIZE(TEXTURE_TILE_SIZE.x * DRAW_SCALE.x, TEXTURE_TILE_SIZE.y * DRAW_SCALE.y);
    const float GRAVITY = -2.f;
    const sf::Vector2f PLAYER_SIZE = TEXTURE_TILE_SIZE;
    const sf::Vector2f GROUND_SPRITE_SIZE(30.f, 30.f);
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mario");

    const float GROUND_TOP = window.getSize().y - 60.f;

    sf::Texture tileSetTexture;
    tileSetTexture.loadFromFile("Resources/MarioTextures_v1.png");
    
    sf::Sprite playerSprite;
    playerSprite.setTexture(tileSetTexture); 
    playerSprite.setTextureRect(sf::IntRect(121, 49, PLAYER_SIZE.x, PLAYER_SIZE.y));
    playerSprite.setScale(DRAW_SCALE);

    sf::Sprite groundSprite;
    groundSprite.setTexture(tileSetTexture);
    groundSprite.setTextureRect(sf::IntRect(247, 596, GROUND_SPRITE_SIZE.x, GROUND_SPRITE_SIZE.y));
    groundSprite.setPosition(0.f, GROUND_TOP);
    groundSprite.setScale(50.f, 50.f);

    sf::FloatRect playerRect(100.f, 400.f, PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.y);
    
    sf::Vector2f playerVelocity(0.f, 0.f);
    float playerSpeed = 200.f;

    float playerJumpSpeed = 500.f;
    bool bIsPlayerOnGround = false;

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
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    if (bIsPlayerOnGround)
                    {
                        bIsPlayerOnGround = false;
                        playerVelocity.y = playerJumpSpeed * clockDeltaSeconds;
                    }
                }
            }
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            playerVelocity.x = -playerSpeed * clockDeltaSeconds;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            playerVelocity.x = playerSpeed * clockDeltaSeconds;
        }
        
        {
            playerVelocity.y += GRAVITY * clockDeltaSeconds;
            
            playerRect.left += playerVelocity.x;
            playerRect.left = handleCollision(playerRect, playerVelocity, level, EOrientation::Horizontal, TILE_SIZE).x;

            playerRect.top -= playerVelocity.y;
            
            const float PLAYER_RECT_BOTTOM = playerRect.top + playerRect.height;
            
            if (PLAYER_RECT_BOTTOM > GROUND_TOP)
            {
                bIsPlayerOnGround = true;
                playerVelocity.y = 0.f;
                playerRect.top = GROUND_TOP - playerRect.height;
            }

            playerVelocity.x = 0.f;
        }

        window.clear();

        //window.draw(groundSprite);
        for(int i = 0; i < level.tiles.size(); ++i)
            for (int j = 0; j < level.tiles[i].size(); ++j)
            {
                const Tile& tile = level.tiles[i][j];

                sf::RectangleShape tileRect;
                tileRect.setPosition(TILE_SIZE.x * j, TILE_SIZE.y * i);
                tileRect.setSize(TILE_SIZE);

                if (tile.textureType == ETileTextureType::Ground)
                {
                    tileRect.setFillColor(sf::Color::Red);
                }
                else if (tile.textureType == ETileTextureType::Empty)
                {
                    tileRect.setFillColor(sf::Color::Blue);
                }

                window.draw(tileRect);
            }

        playerSprite.setPosition(playerRect.left, playerRect.top);
        window.draw(playerSprite);
        window.display();
    }

    return 0;
}