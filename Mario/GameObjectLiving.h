#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "GameObject.h"

class GameObjectLiving : public GameObject
{
public:
	void draw(sf::RenderWindow& window) override;

public:
	sf::Sprite sprite;
	bool bDead = false;
};