#include <SFML/Graphics.hpp>

int main()
{
    const sf::Vector2f DRAW_SCALE(3.f, 3.f);
    const float GRAVITY = -2.f;
    const sf::Vector2f PLAYER_SIZE(30.f, 30.f);
    const sf::Vector2f GROUND_SPRITE_SIZE(16.f, 16.f);
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mario");

    const float GROUND_TOP = window.getSize().y - 200.f;

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
        window.draw(groundSprite);
        playerSprite.setPosition(playerRect.left, playerRect.top);
        window.draw(playerSprite);
        window.display();
    }

    return 0;
}