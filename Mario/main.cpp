#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>

enum class ETileTextureType
{
    Empty = 0,
    Ground = 1
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
    std::map<ETileTextureType, sf::Sprite> tileTextureTypeToSprite;
};

struct LevelDescriptor
{
    std::vector<std::vector<char>> levelSymbols;
    sf::Texture tileSetTexture;
    std::map<ETileTextureType, sf::IntRect> tileTextureTypeToTextureRect;
};

Level createLevel(const LevelDescriptor& LEVEL_DESCRIPTOR, sf::Vector2f tileScale)
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

    for (std::map<ETileTextureType, sf::IntRect>::const_iterator it = LEVEL_DESCRIPTOR.tileTextureTypeToTextureRect.begin(); 
        it != LEVEL_DESCRIPTOR.tileTextureTypeToTextureRect.end(); ++it)
    {
        sf::Sprite sprite;
        sprite.setTexture(LEVEL_DESCRIPTOR.tileSetTexture);
        sprite.setTextureRect(it->second);
        sprite.setScale(tileScale);
        level.tileTextureTypeToSprite[it->first] = sprite;
    }

    return level;
}

enum class EOrientation
{
    Horizontal,
    Vertical
};

struct CollisionResult
{
    bool bIsCollided = false;
    sf::Vector2f newPosition;
};

struct Player
{
    sf::Vector2f playerSize;
    sf::Sprite playerSprite;
    sf::FloatRect playerRect;
    sf::Vector2f playerVelocity;
    float playerSpeed = 200.f;
    float playerJumpSpeed = 400.f;
    bool bIsPlayerOnGround = false;
};

CollisionResult handleCollision(
    const sf::FloatRect& OBJECT_RECT, 
    const sf::Vector2f& OBJECT_VELOCITY,
    const Level& LEVEL, 
    EOrientation orientation,
    const sf::Vector2f& TILE_SIZE)
{
    const int X_LEFT_TILE = OBJECT_RECT.left / TILE_SIZE.x;
    int xRightTile = (OBJECT_RECT.left + OBJECT_RECT.width - 1) / TILE_SIZE.x;

    const int Y_TOP_TILE = OBJECT_RECT.top / TILE_SIZE.y;
    int yBottomTile = (OBJECT_RECT.top + OBJECT_RECT.height - 1) / TILE_SIZE.y;

    CollisionResult result;
    result.newPosition = OBJECT_RECT.getPosition();

    for (int i = Y_TOP_TILE; i <= yBottomTile; ++i)
    {
        for (int j = X_LEFT_TILE; j <= xRightTile; ++j)
        {
            const Tile& TILE = LEVEL.tiles[i][j];
            const sf::Vector2f TILE_POSITION = sf::Vector2f(j * TILE_SIZE.x, i * TILE_SIZE.y);

            if (TILE.collisionType == ETileCollisionType::Collision)
            {
                result.bIsCollided = true;

                if (orientation == EOrientation::Horizontal)
                {
                    if (OBJECT_VELOCITY.x < 0)
                    {
                        result.newPosition = sf::Vector2f(TILE_POSITION.x + TILE_SIZE.x, OBJECT_RECT.top);
                    }
                    else
                    {
                        result.newPosition = sf::Vector2f(TILE_POSITION.x - OBJECT_RECT.width, OBJECT_RECT.top);
                    }
                }
                else
                {
                    if (OBJECT_VELOCITY.y < 0)
                    {
                        result.newPosition = sf::Vector2f(OBJECT_RECT.left, TILE_POSITION.y - OBJECT_RECT.height);
                    }
                    else
                    {
                        result.newPosition = sf::Vector2f(OBJECT_RECT.left, TILE_POSITION.y + TILE_SIZE.y);
                    }
                }
                return result;
            }
        }
    }
    return result;
}

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