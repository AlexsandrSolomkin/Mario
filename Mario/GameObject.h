#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

class GameObject
{
public:
	sf::FloatRect rect;

protected:


private:

};

class GameObjectLivin : public GameObject
{
public:
	sf::Sprite sprite;
	bool bDead = false;
};