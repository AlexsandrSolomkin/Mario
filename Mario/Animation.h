#pragma once

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

class Animation
{
public:
	void update(float deltaSeconds);
	int getCurrentFrameIndex();
	const sf::IntRect& getCurrentFrame();

public:
	sf::Texture texture;

	std::vector<sf::IntRect> frames;

	float speed = 1.f;

private:
	float currentFrameIndex = 0.f;
};
