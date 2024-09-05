#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
	class RenderWindow;
}

class World;

class GameObject
{
public:
	virtual void draw(sf::RenderWindow& window)
	{

	}

public:
	sf::FloatRect rect;
	World* world = nullptr;

protected:
	
private:

};
